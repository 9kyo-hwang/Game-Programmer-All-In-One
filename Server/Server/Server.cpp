﻿#include "pch.h"
#include "SocketManager.h"
#include "ThreadManager.h"
#include "IOCPSession.h"
#include "Service.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ServerPacketHandler.h"

int main()
{
	FSocketManager::Initialize();

	/**
	 * Factory를 넘겨줄 때 기본 세션을 넘기는 건 "컨텐츠단에서 엔진단 코드를 넘기는 것"
	 * 보통 엔진단 코드(IOCPSession)를 그대로 사용하지 않고, 컨텐츠단에서 상속 등으로 확장(GameSession)해서 사용
	 */
	TSharedPtr<ServerService> Service = make_shared<ServerService>(
		FInternetAddr(L"127.0.0.1", 7777),
		make_shared<IOCPCore>(),
		[] { return make_shared<GameSession>(); },
		100);

	assert(Service->Start());

	// Worker Thread 생성
	for (int32 i = 0; i < 5; ++i)
	{
		GThreadManager->Launch([=]
			// Thread Main 함수
			{
				while (true)
				{
					Service->GetCore()->Dispatch();
				}
			});
	}

	// 컨텐츠 코드. AI 로직 등을 작성해서 패킷 작성 후 Broadcast
	while (true)
	{
		vector<BuffData> Data
		{
			BuffData{100, 1.5f},
			BuffData{200, 2.3f},
			BuffData{300, 0.7f}
		};

		TSharedPtr<SendBuffer> Buffer = ServerPacketHandler::Make_S_TEST(1001, 100, 10, Data);
		GSessionManager.Broadcast(Buffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();

	FSocketManager::Clear();

	return 0;
}
