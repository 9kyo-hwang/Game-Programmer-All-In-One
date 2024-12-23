#include "pch.h"
#include "IOCPSession.h"
#include "Service.h"

IOCPSession::IOCPSession()
{
	// 원래는 accept 함수의 반환값을 사용했으나, 비동기 처리이므로 미리 생성
	Socket = FSocketManager::CreateSocket();
}

IOCPSession::~IOCPSession()
{
	FSocketManager::Close(Socket);
}

// 버퍼 또한 포인터처럼 날라가지 않고 유지되도록 각별히 주의해야 함
void IOCPSession::Send(BYTE* Buffer, int32 Len)
{
	/**
	 * 1. 버퍼 관리를 어떻게 할 것인가?
	 * 2. SendEvent 관리는 어떻게 할 것인가? Session이 들고 있도록? 매번 새롭게?
	 */

	// TEMP
	IOCPEvent* SendEvent = new IOCPEvent(ENetworkEvents::Send);
	SendEvent->Owner = shared_from_this();  // Ref Count += 1
	SendEvent->Buffer.resize(Len);
	::memcpy(SendEvent->Buffer.data(), Buffer, Len);  // 임시로 들고 있도록

	// Send는 게임 컨텐츠, 귓말 등 그 누구라도 호출할 수 있어서 LOCK을 걸어줌
	LOCK;
	RegisterSend(SendEvent);
}

bool IOCPSession::Connect()
{
	return RegisterConnect();
}

void IOCPSession::Disconnect(const WCHAR* Cause)
{
	// 중복 처리하지 않도록
	if (!bIsConnected.exchange(false))
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

// Send Event는 동적으로 생성해서 인자로 받아줌
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

	OnRecv(RecvBuffer, NumOfBytes);
	RegisterRecv();  // 낚싯대 걸기
}

void IOCPSession::ProcessSend(IOCPEvent* SendEvent, int32 NumOfBytes)
{
	SendEvent->Owner = nullptr;
	delete SendEvent;

	if (NumOfBytes == 0)  // 받은 데이터가 아예 없다면
	{
		Disconnect(L"Send 0");  // 연결이 끊어진 상태
		return;
	}

	OnSend(NumOfBytes);
	// 다음 Send가 준비됐다는 보장이 없어 낚싯대를 다시 걸 필요가 없음
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
