#pragma once
#include "ClientPacketHandler.h"

// 클라이언트 입장에서는 서버 세션이 됨
class ServerSession : public PacketSession
{
public:
	~ServerSession() override
	{
		//cout << "~ServerSession()" << endl;
	}

	void OnConnected() override
	{
		//cout << "Connected To Server" << endl;
	}

	void OnRecvPacket(BYTE* InBuffer, int32 Len) override
	{
		//cout << "OnRecv Len = " << Len << endl;
		// 이제 패킷 핸들러에게 패킷을 넘겨줌
		// 서버가 보낸 패킷은 이곳으로 들어오게 됨
		ClientPacketHandler::HandlePacket(GetServerSession(), InBuffer, Len);
	}

	void OnSend(int32 Len) override
	{
		//cout << "OnSend Len = " << Len << endl;
	}

	void OnDisconnected() override
	{
		//cout << "Disconnected" << endl;
	}

	SessionRef GetServerSession() { return static_pointer_cast<ServerSession>(shared_from_this()); }
};
