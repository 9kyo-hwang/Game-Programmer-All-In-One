#include "pch.h"
#include "IOCPSession.h"
#include "Service.h"
#include "ThreadManager.h"

// 이제 클라이언트 별 보낼 데이터 관리만 해주면 준비 완료
char SendData[] = "Hello, I am Client!";

// 클라이언트 입장에서는 서버 세션이 됨
class ServerSession : public IOCPSession
{
public:
	~ServerSession() override
	{
		cout << "~ServerSession()" << endl;
	}

	void OnConnected() override
	{
		cout << "Connected To Server" << endl;

		TSharedPtr<SendBuffer> Buffer = make_shared<SendBuffer>(4096);
		Buffer->Copy(SendData, sizeof(SendData));
		Send(Buffer);
	}

	int32 OnRecv(BYTE* InBuffer, int32 Len) override
	{
		cout << "OnRecv Len = " << Len << endl;

		this_thread::sleep_for(0.1s);

		TSharedPtr<SendBuffer> Buffer = make_shared<SendBuffer>(4096);
		Buffer->Copy(SendData, sizeof(SendData));
		Send(Buffer);

		return Len;
	}

	void OnSend(int32 Len) override
	{
		cout << "OnSend Len = " << Len << endl;
	}

	void OnDisconnected() override
	{
		cout << "Disconnected" << endl;
	}

};

int main()
{
	FSocketManager::Initialize();

	this_thread::sleep_for(1s);

	TSharedPtr<ClientService> Service = make_shared<ClientService>(
		FInternetAddr(L"127.0.0.1", 7777),
		make_shared<IOCPCore>(),
		[] { return make_shared<ServerSession>(); },  // TODO: SessionManager
		5
	);

	assert(Service->Start());

	for (int32 i = 0; i < 5; ++i)
	{
		GThreadManager->Launch([=]
			{
				while (true)
				{
					Service->GetCore()->Dispatch();
					this_thread::sleep_for(1s);
				}
			});
	}

	GThreadManager->Join();
	FSocketManager::Clear();

	return 0;
}
