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

int32 GameSession::OnRecv(BYTE* InBuffer, int32 Len)
{
	cout << "OnRecv Len = " << Len << endl;

	// ECHO
	TSharedPtr<SendBuffer> Buffer = make_shared<SendBuffer>(4096);
	Buffer->Copy(InBuffer, Len);
	GSessionManager.Broadcast(Buffer);

	return Len;
}

void GameSession::OnSend(int32 Len)
{
	cout << "OnSend Len = " << Len << endl;
}
