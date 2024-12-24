#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"

GameSessionManager GSessionManager;

void GameSessionManager::Add(TSharedPtr<GameSession> Session)
{
	LOCK;
	Sessions.insert(Session);
}

void GameSessionManager::Remove(TSharedPtr<GameSession> Session)
{
	LOCK;
	Sessions.erase(Session);
}

void GameSessionManager::Broadcast(TSharedPtr<SendBuffer> SendBuffer)
{
	LOCK;
	for (TSharedPtr<GameSession> Session : Sessions)
	{
		Session->Send(SendBuffer);
	}
}
