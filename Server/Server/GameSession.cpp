#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ServerPacketHandler.h"

void GameSession::OnConnected()
{
	GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));

	// �������� Ŭ���̾�Ʈ �븮�ڰ� GameSession�̹Ƿ� ���⼭ ��Ŷ ����
	// ������ �α��� ��Ŷ �� Ŭ���̾�Ʈ�κ��� ������ ���� �ڿ� ó���ϴ� �� ����
	Send(ServerPacketHandler::Make_S_EnterGame());

	// TODO: ���� ����(������)
}

void GameSession::OnDisconnected()
{
	GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnRecvPacket(BYTE* InBuffer, int32 Len)
{
	PacketHeader Header = *reinterpret_cast<PacketHeader*>(InBuffer);
	cout << "Packet [ID: " << Header.ID << ", Size: " << Header.Size << "]" << endl;

	// TODO: ��Ŷ ID�� ���� �б� ó��
}

void GameSession::OnSend(int32 Len)
{
	//cout << "OnSend Len = " << Len << endl;
}
