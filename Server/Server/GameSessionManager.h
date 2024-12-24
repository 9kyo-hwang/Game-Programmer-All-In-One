#pragma once

class GameSession;

class GameSessionManager
{
public:
	void Add(TSharedPtr<GameSession> Session);
	void Remove(TSharedPtr<GameSession> Session);
	void Broadcast(TSharedPtr<SendBuffer> SendBuffer);

private:
	MUTEX;
	unordered_set<TSharedPtr<GameSession>> Sessions;
};

extern GameSessionManager GSessionManager;
