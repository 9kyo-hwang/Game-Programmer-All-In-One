#pragma once
#include "IOCPSession.h"

class GameSession : public PacketSession
{
	using Super = IOCPSession;
public:
	~GameSession() override
	{
		cout << "~GameSession()" << endl;
	}

	void OnConnected() override;
	void OnDisconnected() override;
	void OnRecvPacket(BYTE* InBuffer, int32 Len) override;
	void OnSend(int32 Len) override;
};
