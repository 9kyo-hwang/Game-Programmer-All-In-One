#include "pch.h"
#include "IOCPSession.h"
#include "Service.h"

IOCPSession::IOCPSession()
{
	// ������ accept �Լ��� ��ȯ���� ���������, �񵿱� ó���̹Ƿ� �̸� ����
	Socket = FSocketManager::CreateSocket();
}

IOCPSession::~IOCPSession()
{
	FSocketManager::Close(Socket);
}

// ���� ���� ������ó�� ������ �ʰ� �����ǵ��� ������ �����ؾ� ��
void IOCPSession::Send(BYTE* Buffer, int32 Len)
{
	/**
	 * 1. ���� ������ ��� �� ���ΰ�?
	 * 2. SendEvent ������ ��� �� ���ΰ�? Session�� ��� �ֵ���? �Ź� ���Ӱ�?
	 */

	// TEMP
	IOCPEvent* SendEvent = new IOCPEvent(ENetworkEvents::Send);
	SendEvent->Owner = shared_from_this();  // Ref Count += 1
	SendEvent->Buffer.resize(Len);
	::memcpy(SendEvent->Buffer.data(), Buffer, Len);  // �ӽ÷� ��� �ֵ���

	// Send�� ���� ������, �Ӹ� �� �� ������ ȣ���� �� �־ LOCK�� �ɾ���
	LOCK;
	RegisterSend(SendEvent);
}

bool IOCPSession::Connect()
{
	return RegisterConnect();
}

void IOCPSession::Disconnect(const WCHAR* Cause)
{
	// �ߺ� ó������ �ʵ���
	if (!bIsConnected.exchange(false))
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
		.len = Len32(RecvBuffer),
		.buf = reinterpret_cast<char*>(RecvBuffer),
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
void IOCPSession::RegisterSend(IOCPEvent* SendEvent)
{
	if (!IsConnected())
	{
		return;
	}

	WSABUF Buffer
	{
		.len = static_cast<ULONG>(SendEvent->Buffer.size()),
		.buf = reinterpret_cast<char*>(SendEvent->Buffer.data())
	};
	DWORD NumOfBytes = 0;

	if (SOCKET_ERROR == ::WSASend(
		Socket,
		&Buffer,
		1,
		OUT &NumOfBytes,
		0,
		SendEvent,
		nullptr))
	{
		int32 ErrorCode = ::WSAGetLastError();
		if (ErrorCode != WSA_IO_PENDING)
		{
			HandleError(ErrorCode);
			SendEvent->Owner = nullptr;  // RELEASE REF
			delete SendEvent;
		}
	}
}

void IOCPSession::ProcessConnect()
{
	ConnectEvent.Owner = nullptr;  // RELEASE
	bIsConnected.store(true);

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

	OnRecv(RecvBuffer, NumOfBytes);
	RegisterRecv();  // ���˴� �ɱ�
}

void IOCPSession::ProcessSend(IOCPEvent* SendEvent, int32 NumOfBytes)
{
	SendEvent->Owner = nullptr;
	delete SendEvent;

	if (NumOfBytes == 0)  // ���� �����Ͱ� �ƿ� ���ٸ�
	{
		Disconnect(L"Send 0");  // ������ ������ ����
		return;
	}

	OnSend(NumOfBytes);
	// ���� Send�� �غ�ƴٴ� ������ ���� ���˴븦 �ٽ� �� �ʿ䰡 ����
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
		ProcessSend(Event, NumOfBytes);
		break;
	default:
		break;
	}
}
