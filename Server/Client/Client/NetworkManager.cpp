#include "pch.h"
#include "NetworkManager.h"
#include "ServerSession.h"
#include "Service.h"

void NetworkManager::Initialize()
{
	FSocketManager::Initialize();

	Service = make_shared<ClientService>(
		FInternetAddr(L"127.0.0.1", 7777),
		make_shared<IOCPCore>(),
		[This = this] { return This->CreateSession(); },
		1
	);

	assert(Service->Start());

	// 아래 방식은 네트워크 처리(Dispatch)하는 쓰레드랑 게임 로직 쓰레드가 별도로 동작한다는 의미
	// 백그라운드에서 네트워크 패킷을 받을 수 있는 장점이 있지만, 패킷을 처리할 때 별도로 Lock 처리 등을 해줘야 함
	// 대신, 일감을 Queue에 집어넣는 식으로 해결하기도 함
	// 여기서는 처리를 간단하게 하기 위해 Main Thread에서 Dispatch를 같이 담당하도록 수정
	//for (int32 i = 0; i < 5; ++i)
	//{
	//	GThreadManager->Launch([This = this]
	//		{
	//			while (true)
	//			{
	//				This->Service->GetCore()->Dispatch();
	//			}
	//		});
	//}
}

void NetworkManager::Update()
{
	// 다만 현재 Dispatch는 GetQueuedCompletionStatus()를 호출하고 있는데, 이벤트가 발생할 때까지 무한정 대기가 일어남
	// 이에 대한 해결로 Timeout = 0을 설정함으로서 이벤트가 없는 즉시 함수를 빠져나오도록 설정
	Service->GetCore()->Dispatch(0);
}

TSharedPtr<ServerSession> NetworkManager::CreateSession()
{
	return Session = make_shared<ServerSession>();
}

void NetworkManager::SendPacket(const TSharedPtr<SendBuffer>& InSendBuffer)
{
	if (Session)
	{
		Session->Send(InSendBuffer);
	}
}
