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
		Pred();  // 해당 쓰레드의 메인 함수
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
	// 쓰레드가 하나씩 늘어날 때마다 ID가 1씩 증가하며 할당. Main은 1번
	static TAtomic<uint32> ThreadID = 1;
	TLS_ThreadID = ThreadID.fetch_add(1);
}

void FThreadManager::DestroyTLS()
{
}
