#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "GameZone.h"
#include "ServerPacketHandler.h"

void GameSession::OnConnected()
{
	GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));

	// �������� Ŭ���̾�Ʈ �븮�ڰ� GameSession�̹Ƿ� ���⼭ ��Ŷ ����
	// ������ �α��� ��Ŷ �� Ŭ���̾�Ʈ�κ��� ������ ���� �ڿ� ó���ϴ� �� ����
	Send(ServerPacketHandler::Outgoing_S_EnterGame());

	// TODO: ���� ����(������)
	GZone->Enter(GetGameSession());
}

void GameSession::OnDisconnected()
{
	GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));

	// ���� ����
	GZone->Exit(GetGameSession());
}

void GameSession::OnRecvPacket(BYTE* InBuffer, int32 Len)
{
	PacketHeader Header = *reinterpret_cast<PacketHeader*>(InBuffer);
	cout << "Packet [ID: " << Header.ID << ", Size: " << Header.Size << "]" << endl;

	// TODO: ��Ŷ ID�� ���� �б� ó��
	// Session ����, �� This�� �Ѱ���� �ϴµ� SmartPointer�� �����ϰ� �����Ƿ� shared_from_this()
	// �׷��� ���� ��ȯ�� IOCPObject�� �õ��ϰ� �־ static_pointer_cast�� GameSession �纯ȯ
	ServerPacketHandler::HandlePacket(GetGameSession(), InBuffer, Len);
}

void GameSession::OnSend(int32 Len)
{
	//cout << "OnSend Len = " << Len << endl;
}
