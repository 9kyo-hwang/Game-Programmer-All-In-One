#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"

void GameSession::OnConnected()
{
	GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnDisconnected()
{
	GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnRecvPacket(BYTE* InBuffer, int32 Len)
{
	PacketHeader Header = *reinterpret_cast<PacketHeader*>(InBuffer);
	cout << "Packet [ID: " << Header.ID << ", Size: " << Header.Size << "]" << endl;

	// TODO: 패킷 ID에 따른 분기 처리
}

void GameSession::OnSend(int32 Len)
{
	//cout << "OnSend Len = " << Len << endl;
}
