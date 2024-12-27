#pragma once
#include "Singleton.h"

class ClientService;
class ServerSession;

class NetworkManager : public Singleton<NetworkManager>
{
public:
	~NetworkManager() override {}
	void Initialize();
	void Update();

	TSharedPtr<ServerSession> CreateSession();
	void SendPacket(const TSharedPtr<SendBuffer>& InSendBuffer);

private:
	TSharedPtr<ClientService> Service;
	TSharedPtr<ServerSession> Session;
};

