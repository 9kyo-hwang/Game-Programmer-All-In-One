#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "GameZone.h"
#include "ServerPacketHandler.h"

void GameSession::OnConnected()
{
	GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));

	// 서버에서 클라이언트 대리자가 GameSession이므로 여기서 패킷 전송
	// 원래는 로그인 패킷 등 클라이언트로부터 정보를 받은 뒤에 처리하는 게 맞음
	Send(ServerPacketHandler::Outgoing_S_EnterGame());

	// TODO: 게임 입장(컨텐츠)
	GZone->Enter(GetGameSession());
}

void GameSession::OnDisconnected()
{
	GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));

	// 게임 퇴장
	GZone->Exit(GetGameSession());
}

void GameSession::OnRecvPacket(BYTE* InBuffer, int32 Len)
{
	PacketHeader Header = *reinterpret_cast<PacketHeader*>(InBuffer);
	cout << "Packet [ID: " << Header.ID << ", Size: " << Header.Size << "]" << endl;

	// TODO: 패킷 ID에 따른 분기 처리
	// Session 정보, 즉 This를 넘겨줘야 하는데 SmartPointer로 관리하고 있으므로 shared_from_this()
	// 그런데 현재 변환이 IOCPObject로 시도하고 있어서 static_pointer_cast로 GameSession 재변환
	ServerPacketHandler::HandlePacket(GetGameSession(), InBuffer, Len);
}

void GameSession::OnSend(int32 Len)
{
	//cout << "OnSend Len = " << Len << endl;
}
