#pragma once
#include <functional>

class FThreadManager
{
public:
	FThreadManager();
	~FThreadManager();

	void Launch(function<void()> Pred);
	void Join();

private:
	static void InitializeTLS();
	static void DestroyTLS();

private:
	FCriticalSection Mutex;
	vector<thread> Threads;
};

