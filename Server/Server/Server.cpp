#include "pch.h"
#include "FThreadManager.h"

using namespace std;

void Test()
{
	cout << "Hi! I'm a Thread " << TLS_ThreadID << "\n";

	while (true)
	{
		
	}
}

int main()
{
    for (int32 i = 0; i < 10; ++i)
    {
		GThreadManager->Launch(Test);
    }

	GThreadManager->Join();

	return 0;
}