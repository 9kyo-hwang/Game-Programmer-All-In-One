#include "pch.h"
#include "ThreadManager.h"

FThreadManager::FThreadManager()
{
	// Main Thread
	InitializeTLS();
}

FThreadManager::~FThreadManager()
{
	Join();
}

void FThreadManager::Launch(function<void()> Pred)
{
	FScopeLock Lock(Mutex);
	Threads.emplace_back([=]
	{
		InitializeTLS();
		Pred();  // �ش� �������� ���� �Լ�
		DestroyTLS();
	});
}

void FThreadManager::Join()
{
	for (thread& Thread : Threads)
	{
		if (Thread.joinable())
		{
			Thread.join();
		}
	}

	Threads.clear();
}

void FThreadManager::InitializeTLS()
{
	// �����尡 �ϳ��� �þ ������ ID�� 1�� �����ϸ� �Ҵ�. Main�� 1��
	static TAtomic<uint32> ThreadID = 1;
	TLS_ThreadID = ThreadID.fetch_add(1);
}

void FThreadManager::DestroyTLS()
{
}
