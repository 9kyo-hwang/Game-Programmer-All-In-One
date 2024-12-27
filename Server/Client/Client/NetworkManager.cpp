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

	// �Ʒ� ����� ��Ʈ��ũ ó��(Dispatch)�ϴ� ������� ���� ���� �����尡 ������ �����Ѵٴ� �ǹ�
	// ��׶��忡�� ��Ʈ��ũ ��Ŷ�� ���� �� �ִ� ������ ������, ��Ŷ�� ó���� �� ������ Lock ó�� ���� ����� ��
	// ���, �ϰ��� Queue�� ����ִ� ������ �ذ��ϱ⵵ ��
	// ���⼭�� ó���� �����ϰ� �ϱ� ���� Main Thread���� Dispatch�� ���� ����ϵ��� ����
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
	// �ٸ� ���� Dispatch�� GetQueuedCompletionStatus()�� ȣ���ϰ� �ִµ�, �̺�Ʈ�� �߻��� ������ ������ ��Ⱑ �Ͼ
	// �̿� ���� �ذ�� Timeout = 0�� ���������μ� �̺�Ʈ�� ���� ��� �Լ��� ������������ ����
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
