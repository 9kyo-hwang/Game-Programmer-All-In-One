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
	
	SessionRef GetGameSession() { return static_pointer_cast<GameSession>(shared_from_this());  }
};
 