#pragma once

class AActor;

class GameZone : public TSharedFromThis<GameZone>
{
public:
	GameZone();
	virtual ~GameZone();

	void Initialize();
	void Update();

	void Enter(SessionRef EnterSession);
	void Exit(SessionRef ExitSession);

	TSharedPtr<GameZone> GetZone() { return shared_from_this(); }
	TSharedPtr<AActor> FindActorBy(uint64 Id);

public:
	void AddActor(TSharedPtr<AActor> New);
	void RemoveActor(uint64 Id);
	void Broadcast(const TSharedPtr<SendBuffer>& SendBuffer);

private:
	// 각각의 존은 ID로 플레이어들과 몬스터들을 구분함
	unordered_map<uint64, TSharedPtr<class APlayer>> Players;
	unordered_map<uint64, TSharedPtr<class AMonster>> Monsters;
};

extern TSharedPtr<GameZone> GZone;
