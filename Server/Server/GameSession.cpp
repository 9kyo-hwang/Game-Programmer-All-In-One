#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ServerPacketHandler.h"

void GameSession::OnConnected()
{
	GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));

	// 서버에서 클라이언트 대리자가 GameSession이므로 여기서 패킷 전송
	// 원래는 로그인 패킷 등 클라이언트로부터 정보를 받은 뒤에 처리하는 게 맞음
	Send(ServerPacketHandler::Make_S_EnterGame());

	// TODO: 게임 입장(컨텐츠)
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
