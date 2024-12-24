#include "pch.h"
#include "IOCPSession.h"
#include "Service.h"

IOCPSession::IOCPSession()
	: RecvBuffer(BufferSize)
{
	// ������ accept �Լ��� ��ȯ���� ���������, �񵿱� ó���̹Ƿ� �̸� ����
	Socket = FSocketManager::CreateSocket();
}

IOCPSession::~IOCPSession()
{
	FSocketManager::Close(Socket);
}

// ���� ���� ������ó�� ������ �ʰ� �����ǵ��� ������ �����ؾ� ��
void IOCPSession::Send(TSharedPtr<SendBuffer> InSendBuffer)
{
	bool IsFirstThread = false;  // ��� ���� �޸𸮶� �Ű� �Ƚᵵ ��
	{
		// Send�� ���� ������, �Ӹ� �� �� ������ ȣ���� �� �־ LOCK�� �ɾ���
		LOCK;
		SendQueue.push(InSendBuffer);

		// ���� ���� false, true�� exchange�ϱ� ���� ���� false -> ���� ���ʷ� ���� ������
		IsFirstThread = bSendRegistered.exchange(true) == false;
	}

	// Send�� ���� �����尡 ���� ������ ������� ���ÿ� ���� �� ����
	// �׷��� ���˴�� ���� ������ �ϴ°�? -> ó�� ���� �����尡 ������!
	if (IsFirstThread)
	{
		RegisterSend();
	}
}

bool IOCPSession::Connect()
{
	return RegisterConnect();
}

void IOCPSession::Disconnect(const WCHAR* Cause)
{
	// �ߺ� ó������ �ʵ���
	if (!bConnected.exchange(false))
	{
		return;
	}

	// TEMP
	wcout << "Disconnect: " << Cause << endl;

	OnDisconnected();  // �������ܿ��� ������
	GetService()->ReleaseSession(GetSession());
	RegisterDisconnect();
}

bool IOCPSession::RegisterConnect()
{
	if (IsConnected())
	{
		return false;
	}

	if (GetService()->GetType() != EServices::Client)
	{
		return false;
	}

	if (!FSocketManager::SetReuseAddr(Socket, true))
	{
		return false;
	}

	if (!FSocketManager::Bind(Socket, 0/*���� ��Ʈ*/))
	{
		return false;
	}

	ConnectEvent.Initialize();
	ConnectEvent.Owner = shared_from_this();  // Ref Count += 1;

	DWORD NumOfBytes = 0;
	SOCKADDR_IN Addr = GetService()->GetAddr().GetAddr();
	if (!FSocketManager::ConnectEx(
		Socket, 
		reinterpret_cast<SOCKADDR*>(&Addr), 
		sizeof(Addr), 
		nullptr,
		0,
		&NumOfBytes,
		&ConnectEvent))
	{
		if (::WSAGetLastError() != WSA_IO_PENDING)
		{
			ConnectEvent.Owner = nullptr;  // Release Reference
			return false;
		}
	}

	return true;
}

bool IOCPSession::RegisterDisconnect()
{
	DisconnectEvent.Initialize();
	DisconnectEvent.Owner = shared_from_this();  // ADD REF

	if (!FSocketManager::DisconnectEx(
		Socket,
		&DisconnectEvent,
		TF_REUSE_SOCKET,
		0))
	{
		if (::WSAGetLastError() != WSA_IO_PENDING)
		{
			DisconnectEvent.Owner = nullptr;  // Release Reference
			return false;
		}
	}

	return true;
}

void IOCPSession::RegisterRecv()
{
	if (!IsConnected())
	{
		return;
	}

	RecvEvent.Initialize();
	RecvEvent.Owner = shared_from_this();

	WSABUF Buffer
	{
		.len = static_cast<ULONG>(RecvBuffer.GetFreeSize()),
		.buf = reinterpret_cast<char*>(RecvBuffer.GetWritePos()),
	};

	DWORD NumOfBytes = 0;
	DWORD Flags = 0;
	if (SOCKET_ERROR == ::WSARecv(
		Socket,
		&Buffer,
		1,
		OUT &NumOfBytes,
		OUT &Flags,
		&RecvEvent,
		nullptr))
	{
		int32 ErrorCode = ::WSAGetLastError();
		if (ErrorCode != WSA_IO_PENDING)
		{
			HandleError(ErrorCode);
			RecvEvent.Owner = nullptr;  // RELEASE REF
		}
	}
}

// Send Event�� �������� �����ؼ� ���ڷ� �޾���
void IOCPSession::RegisterSend()
{
	if (!IsConnected())
	{
		return;
	}

	SendEvent.Initialize();
	SendEvent.Owner = shared_from_this();

	// ���� �����͸� Event�� ���
	{
		// STL�� queue�� thread-safe���� ����
		// �ٸ� �����尡 send�� ȣ���� queue�� �����͸� push�ϰ� ���� �� �����Ƿ� LOCK
		LOCK;

		int32 WriteSize = 0;
		while (!SendQueue.empty())
		{
			TSharedPtr<SendBuffer> SendBuffer = SendQueue.front();
			WriteSize += SendBuffer->Num();

			// TODO: ���� üũ

			SendQueue.pop();
			SendEvent.SendBufferList.push_back(SendBuffer);
		}
	}

	// Scatter-Gather ������� �����͸� ��Ƽ� �� ���� ����
	vector<WSABUF> Buffers;
	Buffers.reserve(SendEvent.SendBufferList.size());
	for (TSharedPtr<SendBuffer> SendBuffer : SendEvent.SendBufferList)
	{
		Buffers.emplace_back(
			static_cast<ULONG>(SendBuffer->Num()),
			reinterpret_cast<char*>(SendBuffer->GetData())
		);
	}

	DWORD NumOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(
		Socket,
		Buffers.data(),
		Buffers.size(),
		OUT &NumOfBytes,
		0,
		&SendEvent,
		nullptr))
	{
		int32 ErrorCode = ::WSAGetLastError();
		if (ErrorCode != WSA_IO_PENDING)
		{
			HandleError(ErrorCode);

			SendEvent.Owner = nullptr;  // RELEASE REF
			SendEvent.SendBufferList.clear();
			bSendRegistered.store(false);
		}
	}
}

void IOCPSession::ProcessConnect()
{
	ConnectEvent.Owner = nullptr;  // RELEASE
	bConnected.store(true);

	GetService()->AddSession(GetSession());  // ���� ���
	OnConnected();
	RegisterRecv();  // �ٽ� ���˴븦 �ɾ� ����
}

void IOCPSession::ProcessDisconnect()
{
	DisconnectEvent.Owner = nullptr;  // RELEASE
}

void IOCPSession::ProcessRecv(int32 NumOfBytes)
{
	RecvEvent.Owner = nullptr;  // RELEASE
	if (NumOfBytes == 0)  // ���� �����Ͱ� �ƿ� ���ٸ�
	{
		Disconnect(L"Recv 0");  // ������ ������ ����
		return;
	}

	if (!RecvBuffer.OnWrite(NumOfBytes))
	{
		Disconnect(L"OnWrite Overflow");
		return;
	}

	// �������ܿ��� ó���� �� �ִ� ������ ũ��
	int32 DataSize = RecvBuffer.GetDataSize();
	int32 ProcessLen = OnRecv(RecvBuffer.GetReadPos(), DataSize);
	if (ProcessLen < 0
		|| DataSize < ProcessLen
		|| !RecvBuffer.OnRead(ProcessLen))
	{
		Disconnect(L"OnRead Overflow");
		return;
	}

	RecvBuffer.Clean();  // Ŀ�� ����
	RegisterRecv();  // ���˴� �ɱ�
}

void IOCPSession::ProcessSend(int32 NumOfBytes)
{
	SendEvent.Owner = nullptr;
	SendEvent.SendBufferList.clear();

	if (NumOfBytes == 0)  // ���� �����Ͱ� �ƿ� ���ٸ�
	{
		Disconnect(L"Send 0");  // ������ ������ ����
		return;
	}

	OnSend(NumOfBytes);
	// ���� Send�� �غ�ƴٴ� ������ ���� ���˴븦 �ٽ� �� �ʿ䰡 ����

	/**
	 * Send�� ���ʷ� ���� �����尡 ���⿡ �������� ������,
	 * �ڵ���� ������鿡 ���� �ڴʰ� Queue�� SendBuffer�� �߰��Ǿ�
	 * ���� �����尡 Queue�� �� ó���ߴٰ� ������ ���� ����
	 * �� ������ Queue�� �����Ͱ� �������� ���� ����
	 * �̿� ���� �ڵ鸵�� ����� ��
	 */

	bool IsFirstThread = false;  // ���� �޸�
	{
		LOCK;

		if (SendQueue.empty())
		{
			// ��¥�� SendQueue�� ����ٸ� ������� ������
			bSendRegistered.store(false);
		}
		else
		{
			IsFirstThread = true;
		}
	}

	// ǥ�� mutex�� ���� LOCK�� ������� �ʾ�, �̸� ȸ���ϱ� ���� �ڵ� 
	if (IsFirstThread)
	{
		RegisterSend();
	}
}

void IOCPSession::HandleError(int32 ErrorCode)
{
	switch (ErrorCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"HandleError");
		break;
	default:
		// TODO: Log
		cout << "Handle Error: " << ErrorCode << endl;
		break;
	}
}

PacketSession::PacketSession()
{
}

PacketSession::~PacketSession()
{
}

// [size(2byte)][id(2byte)][data...][size(2byte)][id(2byte)][data...]
// ��Ŷ ���� �õ�
int32 PacketSession::OnRecv(BYTE* InBuffer, int32 Len)
{
	int32 ProcessLen = 0;
	while (true)
	{
		int32 DataSize = Len - ProcessLen;
		if (DataSize < sizeof(PacketHeader))  // ���(4����Ʈ)���� ������ �Ľ� �Ұ���
		{
			break;
		}

		// ProcessLen ���������� �ּҸ� PacketHeader�� �����ϰ���
		// �� �� �ּҸ� PacketHeader�� �����ͷ� ĳ����, �װ��� ��(�� �� *)�� ������
		PacketHeader Header = *reinterpret_cast<PacketHeader*>(&InBuffer[ProcessLen]);
		if (DataSize < Header.Size)  // ����� ��ϵ� ��Ŷ ũ�⸦ �Ľ��� �� �־�� ��
		{
			break;
		}

		OnRecvPacket(&InBuffer[ProcessLen], Header.Size);  // �������ܿ��� ó��

		ProcessLen += Header.Size;
	}

	return ProcessLen;
}

HANDLE IOCPSession::GetHandle()
{
	return reinterpret_cast<HANDLE>(Socket);
}

void IOCPSession::Dispatch(IOCPEvent* Event, int32 NumOfBytes)
{
	// TODO
	switch (Event->Type)
	{
	case ENetworkEvents::Connect:
		ProcessConnect();
		break;
	case ENetworkEvents::Disconnect:
		ProcessDisconnect();
		break;
	case ENetworkEvents::Recv:
		ProcessRecv(NumOfBytes);
		break;
	case ENetworkEvents::Send:
		ProcessSend(NumOfBytes);
		break;
	default:
		break;
	}
}
