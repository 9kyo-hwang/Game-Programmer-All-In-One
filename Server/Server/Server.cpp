#include "pch.h"
#include "SocketManager.h"
#include "ThreadManager.h"
#include "IOCPSession.h"
#include "Service.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "GameZone.h"
#include "ServerPacketHandler.h"

int main()
{
	FSocketManager::Initialize();
	GZone->Initialize();  // 보통 어디 매니저에서 호출하는 것이 좋긴 한데

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

	//// Worker Thread 생성
	//for (int32 i = 0; i < 5; ++i)
	//{
	//	GThreadManager->Launch([=]
	//		// Thread Main 함수
	//		{
	//			while (true)
	//			{
	//				Service->GetCore()->Dispatch();
	//			}
	//		});
	//}

	// AI 로직 등 GameZone의 Update는 Server의 MainThread가, WorkerThread는 패킷 핑퐁 처리
	while (true)
	{
		// Client는 NetworkManager 또한 MainThread에 물려놓은 상태
		// 그러면 Server는?
		// 1. Enter/Exit 등에 전부 LOCK 처리 -> 그러면 싱글 쓰레드 쓰는 거랑 다를 바 없음
		// 2. Job/Task 단위를 정의해서 Queue에 넣고 처리하기
		// 우리는 단순하게 하기 위해 Client처럼 MainThread에서 다 처리
		Service->GetCore()->Dispatch(0);
		GZone->Update();  // 원래는 0.1초마다 한 번씩 업데이트해줘도 충분
	}

	GThreadManager->Join();

	FSocketManager::Clear();

	return 0;
}