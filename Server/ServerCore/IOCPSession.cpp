#include "pch.h"
#include "IOCPSession.h"
#include "Service.h"

IOCPSession::IOCPSession()
	: RecvBuffer(BufferSize)
{
	// 원래는 accept 함수의 반환값을 사용했으나, 비동기 처리이므로 미리 생성
	Socket = FSocketManager::CreateSocket();
}

IOCPSession::~IOCPSession()
{
	FSocketManager::Close(Socket);
}

// 버퍼 또한 포인터처럼 날라가지 않고 유지되도록 각별히 주의해야 함
void IOCPSession::Send(TSharedPtr<SendBuffer> InSendBuffer)
{
	bool IsFirstThread = false;  // 얘는 스택 메모리라 신경 안써도 됨
	{
		// Send는 게임 컨텐츠, 귓말 등 그 누구라도 호출할 수 있어서 LOCK을 걸어줌
		LOCK;
		SendQueue.push(InSendBuffer);

		// 최초 값이 false, true로 exchange하기 이전 값이 false -> 내가 최초로 들어온 쓰레드
		IsFirstThread = bSendRegistered.exchange(true) == false;
	}

	// Send는 여러 쓰레드가 단일 세션을 대상으로 동시에 보낼 수 있음
	// 그러면 낚싯대는 누가 던져야 하는가? -> 처음 들어온 쓰레드가 던지자!
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
	// 중복 처리하지 않도록
	if (!bConnected.exchange(false))
	{
		return;
	}

	// TEMP
	wcout << "Disconnect: " << Cause << endl;

	OnDisconnected();  // 컨텐츠단에서 재정의
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

	if (!FSocketManager::Bind(Socket, 0/*남는 포트*/))
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

// Send Event는 동적으로 생성해서 인자로 받아줌
void IOCPSession::RegisterSend()
{
	if (!IsConnected())
	{
		return;
	}

	SendEvent.Initialize();
	SendEvent.Owner = shared_from_this();

	// 보낼 데이터를 Event에 등록
	{
		// STL의 queue는 thread-safe하지 않음
		// 다른 쓰레드가 send를 호출해 queue에 데이터를 push하고 있을 수 있으므로 LOCK
		LOCK;

		int32 WriteSize = 0;
		while (!SendQueue.empty())
		{
			TSharedPtr<SendBuffer> SendBuffer = SendQueue.front();
			WriteSize += SendBuffer->Num();

			// TODO: 예외 체크

			SendQueue.pop();
			SendEvent.SendBufferList.push_back(SendBuffer);
		}
	}

	// Scatter-Gather 방식으로 데이터를 모아서 한 번에 전송
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

	GetService()->AddSession(GetSession());  // 세션 등록
	OnConnected();
	RegisterRecv();  // 다시 낚싯대를 걸어 놓자
}

void IOCPSession::ProcessDisconnect()
{
	DisconnectEvent.Owner = nullptr;  // RELEASE
}

void IOCPSession::ProcessRecv(int32 NumOfBytes)
{
	RecvEvent.Owner = nullptr;  // RELEASE
	if (NumOfBytes == 0)  // 받은 데이터가 아예 없다면
	{
		Disconnect(L"Recv 0");  // 연결이 끊어진 상태
		return;
	}

	if (!RecvBuffer.OnWrite(NumOfBytes))
	{
		Disconnect(L"OnWrite Overflow");
		return;
	}

	// 컨텐츠단에서 처리할 수 있는 데이터 크기
	int32 DataSize = RecvBuffer.GetDataSize();
	int32 ProcessLen = OnRecv(RecvBuffer.GetReadPos(), DataSize);
	if (ProcessLen < 0
		|| DataSize < ProcessLen
		|| !RecvBuffer.OnRead(ProcessLen))
	{
		Disconnect(L"OnRead Overflow");
		return;
	}

	RecvBuffer.Clean();  // 커서 정리
	RegisterRecv();  // 낚싯대 걸기
}

void IOCPSession::ProcessSend(int32 NumOfBytes)
{
	SendEvent.Owner = nullptr;
	SendEvent.SendBufferList.clear();

	if (NumOfBytes == 0)  // 받은 데이터가 아예 없다면
	{
		Disconnect(L"Send 0");  // 연결이 끊어진 상태
		return;
	}

	OnSend(NumOfBytes);
	// 다음 Send가 준비됐다는 보장이 없어 낚싯대를 다시 걸 필요가 없음

	/**
	 * Send에 최초로 들어온 쓰레드가 여기에 진입했을 시점에,
	 * 뒤따라온 쓰레드들에 의해 뒤늦게 Queue에 SendBuffer가 추가되어
	 * 최초 쓰레드가 Queue를 다 처리했다고 착각할 수도 있음
	 * 즉 여전히 Queue에 데이터가 남아있을 수도 있음
	 * 이에 대한 핸들링을 해줘야 함
	 */

	bool IsFirstThread = false;  // 스택 메모리
	{
		LOCK;

		if (SendQueue.empty())
		{
			// 진짜로 SendQueue가 비었다면 원래대로 돌려줌
			bSendRegistered.store(false);
		}
		else
		{
			IsFirstThread = true;
		}
	}

	// 표준 mutex는 이중 LOCK을 허용하지 않아, 이를 회피하기 위한 코드 
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
// 패킷 조립 시도
int32 PacketSession::OnRecv(BYTE* InBuffer, int32 Len)
{
	int32 ProcessLen = 0;
	while (true)
	{
		int32 DataSize = Len - ProcessLen;
		if (DataSize < sizeof(PacketHeader))  // 헤더(4바이트)보다 작으면 파싱 불가능
		{
			break;
		}

		// ProcessLen 지점부터의 주소를 PacketHeader로 간주하겠음
		// 이 때 주소를 PacketHeader의 포인터로 캐스팅, 그것의 값(맨 앞 *)을 꺼내옴
		PacketHeader Header = *reinterpret_cast<PacketHeader*>(&InBuffer[ProcessLen]);
		if (DataSize < Header.Size)  // 헤더에 기록된 패킷 크기를 파싱할 수 있어야 함
		{
			break;
		}

		OnRecvPacket(&InBuffer[ProcessLen], Header.Size);  // 컨텐츠단에서 처리

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
