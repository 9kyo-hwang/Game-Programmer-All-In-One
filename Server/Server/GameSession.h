#pragma once
#include "IOCPSession.h"

class GameSession : public IOCPSession
{
	using Super = IOCPSession;
public:
	~GameSession() override
	{
		cout << "~GameSession()" << endl;
	}

	void OnConnected() override;
	void OnDisconnected() override;
	int32 OnRecv(BYTE* InBuffer, int32 Len) override;
	void OnSend(int32 Len) override;
};
