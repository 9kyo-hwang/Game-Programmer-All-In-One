#include "pch.h"
#include "SocketManager.h"
#include "IOCPListener.h"
#include "ThreadManager.h"
#include "IOCPSession.h"
#include "Service.h"

class GameSession : public IOCPSession
{
public:
	// TODO
	~GameSession() override
	{
		cout << "~GameSession()" << endl;
	}

	int32 OnRecv(BYTE* Buffer, int32 Len) override
	{
		cout << "OnRecv Len = " << Len << endl;
		Send(Buffer, Len);  // 우리가 받은 걸 그대로 다시 돌려줌
		return Len;
	}

	void OnSend(int32 Len) override
	{
		cout << "OnSend Len = " << Len << endl;
	}
};

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

	GThreadManager->Join();

	FSocketManager::Clear();

	return 0;
}