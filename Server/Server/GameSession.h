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

public:
	// 각 세션이 담당하던 플레이어와 존에 대한 정보를 들고 있어야 ExitZone에서 정보 알림이 가능
	TWeakPtr<class GameZone> Zone;
	TWeakPtr<class APlayer> Player;
};
 