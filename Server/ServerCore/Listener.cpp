#include "pch.h"
#include "Listener.h"
#include "IOCPEvent.h"
#include "IOCPSession.h"

IOCPListener::~IOCPListener()
{
	FSocketManager::Close(Socket);
	for (IOCPEvent* Event : Events)
	{
		// TODO
		delete Event;
	}
}

bool IOCPListener::Accept(FInternetAddr Addr)
{
	Socket = FSocketManager::CreateSocket();
	if (Socket == INVALID_SOCKET)
	{
		return false;
	}

	if (!GIOCPCore.Register(this))
	{
		return false;
	}

	if (!FSocketManager::SetReuseAddr(Socket, true))
	{
		return false;
	}

	if (!FSocketManager::SetLinger(Socket, false, 0))
	{
		return false;
	}

	if (!FSocketManager::Bind(Socket, Addr))
	{
		return false;
	}

	if (!FSocketManager::Listen(Socket))
	{
		return false;
	}

	// 임시로 낚싯대는 1개만
	constexpr int32 AcceptCount = 1;
	for (int32 i = 0; i < AcceptCount; ++i)
	{
		IOCPEvent* Event = new IOCPEvent(ENetworkEvents::Accept);
		Events.push_back(Event);
		Register(Event);
	}

	return false;
}

void IOCPListener::Close()
{
	FSocketManager::Close(Socket);
}

HANDLE IOCPListener::GetHandle()
{
	return reinterpret_cast<HANDLE>(Socket);
}

// CP Queue에 들어있는 이벤트를 꺼내서 실행하는 단계
void IOCPListener::Dispatch(IOCPEvent* Event, int32 NumOfBytes)
{
	assert(Event->Type == ENetworkEvents::Accept);
	Process(Event);
}

// 낚싯대를 던지는 과정
void IOCPListener::Register(IOCPEvent* Event)
{
	IOCPSession* Session = new IOCPSession();
	Event->Initialize();  // 초기값을 밀어줘야 함
	Event->Session = Session;

	DWORD ReceivedBytes = 0;
	// 원래 accept은 Socket을 바로 반환해줘서 그것을 기반으로 Session을 만들어 처리
	// 하지만 AcceptEx는 비동기 방식이기 때문에 언제 완료될 지 모름. 그래서 Session을 우선 만들어서 넘겨줌
	// RecvBuffer에 클라이언트 관련 정보를 저장하는 구조
	if (!FSocketManager::AcceptEx(
		Socket, 
		Session->GetSocket(), 
		Session->RecvBuffer, 
		0, 
		sizeof(SOCKADDR_IN) + 16, 
		sizeof(SOCKADDR_IN) + 16, 
		OUT &ReceivedBytes, 
		static_cast<LPOVERLAPPED>(Event)))
	{
		const int32 ErrorCode = ::WSAGetLastError();
		if (ErrorCode != WSA_IO_PENDING)
		{
			// 일단 다시 Accept
			Register(Event);
		}
	}
}

void IOCPListener::Process(IOCPEvent* Event)
{
	IOCPSession* Session = Event->Session;
	if (!FSocketManager::SetUpdateAcceptContext(Session->GetSocket(), Socket))
	{
		Register(Event);
		return;
	}

	SOCKADDR_IN Addr{};
	int32 SizeOfAddr = sizeof(Addr);
	if (SOCKET_ERROR == ::getpeername(Session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&Addr), &SizeOfAddr))
	{
		Register(Event);
		return;
	}

	Session->SetIp(FInternetAddr(Addr));
	cout << "Client Connected!" << endl;

	Register(Event);
}
