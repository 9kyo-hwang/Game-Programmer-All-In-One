#include "pch.h"
#include "SocketManager.h"
#include "Listener.h"
#include "ThreadManager.h"

/**
 * 소켓 생성
 * 포트 bind
 * 소켓 listen(TCP only)
 * 클라이언트 accept(TCP only)
 * 통신
 */

/*
* Select
* 1. Read[] Write[] Except[] 관찰 대상 선정
* 2. ::select(ReadSet, WriteSet, ExceptSet); -> 관찰 시작
* 3. 소켓이 1개라도 준비되면 즉시 리턴, 나머지 소켓은 알아서 제거
* 4. 남은 소켓을 체크한 후 이어서 진행
*/

/*
 * WSAEventSelect
 * 소켓과 관련된 네트워크 이벤트를 [이벤트 객체]로 탐지
 * 생성: WSACreateEvent(Manual Reset + Non-Signaled 상태로 시작)
 * 삭제: WSACloseEvent
 * 이벤트 감지: WSAWaitForMultiplesEvent
 * 이벤트 확인: WSAEnumNetworkEvents
 */

/*
 * Overlapped IO
 */

int main()
{
	FSocketManager::Initialize();

	IOCPListener Listener;
	Listener.Accept(FInternetAddr(L"127.0.0.1", 7777));

	// Worker Thread 생성
	for (int32 i = 0; i < 5; ++i)
	{
		GThreadManager->Launch([=]
			// Thread Main 함수
			{
				while (true)
				{
					GIOCPCore.Dispatch();
				}
			});
	}

	GThreadManager->Join();

	FSocketManager::Clear();

	return 0;
}