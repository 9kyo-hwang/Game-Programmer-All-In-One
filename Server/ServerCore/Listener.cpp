#include "pch.h"
#include "Listener.h"
#include "IOCPEvent.h"
#include "IOCPSession.h"
#include "Service.h"

IOCPListener::~IOCPListener()
{
	FSocketManager::Close(Socket);
	for (IOCPEvent* Event : Events)
	{
		// TODO
		delete Event;
	}
}

bool IOCPListener::Accept(TSharedPtr<ServerService> InService)
{
	Service = InService;
	if (!Service)
	{
		return false;
	}

	Socket = FSocketManager::CreateSocket();
	if (Socket == INVALID_SOCKET)
	{
		return false;
	}

	// �� Service�� ��� �ִ� Core�� �´� Register
	if (!Service->GetCore()->Register(shared_from_this()))
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

	if (!FSocketManager::Bind(Socket, Service->GetAddr()))
	{
		return false;
	}

	if (!FSocketManager::Listen(Socket))
	{
		return false;
	}

	// �ӽ÷� ���˴�� 1����
	constexpr int32 AcceptCount = 1;
	for (int32 i = 0; i < AcceptCount; ++i)
	{
		IOCPEvent* Event = new IOCPEvent(ENetworkEvents::Accept);
		Event->Owner = shared_from_this();  // �ݵ�� Owner�� ��������� Event�� ����ִ� ���� Owner�� �������� �ʰ� ��
		Events.push_back(Event);
		Register(Event);
	}

	return true;
}

void IOCPListener::Close()
{
	FSocketManager::Close(Socket);
}

HANDLE IOCPListener::GetHandle()
{
	return reinterpret_cast<HANDLE>(Socket);
}

// CP Queue�� ����ִ� �̺�Ʈ�� ������ �����ϴ� �ܰ�
void IOCPListener::Dispatch(IOCPEvent* Event, int32 NumOfBytes)
{
	assert(Event->Type == ENetworkEvents::Accept);
	Process(Event);
}

// ���˴븦 ������ ����
void IOCPListener::Register(IOCPEvent* Event)
{
	TSharedPtr<IOCPSession> Session = make_shared<IOCPSession>();

	Event->Initialize();  // �ʱⰪ�� �о���� ��
	Event->Session = Session;

	DWORD ReceivedBytes = 0;
	// ���� accept�� Socket�� �ٷ� ��ȯ���༭ �װ��� ������� Session�� ����� ó��
	// ������ AcceptEx�� �񵿱� ����̱� ������ ���� �Ϸ�� �� ��. �׷��� Session�� �켱 ���� �Ѱ���
	// RecvBuffer�� Ŭ���̾�Ʈ ���� ������ �����ϴ� ����
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
			// �ϴ� �ٽ� Accept
			Register(Event);
		}
	}
}

void IOCPListener::Process(IOCPEvent* Event)
{
	TSharedPtr<IOCPSession> Session = Event->Session;
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
