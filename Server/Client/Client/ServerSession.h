#pragma once
#include "ClientPacketHandler.h"

// Ŭ���̾�Ʈ ���忡���� ���� ������ ��
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
		// ���� ��Ŷ �ڵ鷯���� ��Ŷ�� �Ѱ���
		// ������ ���� ��Ŷ�� �̰����� ������ ��
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
