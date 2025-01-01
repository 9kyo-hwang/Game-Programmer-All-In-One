#pragma once

#include "TileSet.h"
#include "../Client/Client/TimerManager.h"

class AActor;
class APawn;
class APlayer;
class AMonster;

class GameZone : public TSharedFromThis<GameZone>
{
public:
	struct AStarNode
	{
		AStarNode(int32 InCost, Vector2Int InPos)
			: Cost(InCost)
			, Pos(InPos)
		{}

		bool operator<(const AStarNode& Other) const { return Cost < Other.Cost; }
		bool operator>(const AStarNode& Other) const { return Cost > Other.Cost; }
		bool operator==(const AStarNode& Other) const { return Cost == Other.Cost; }

		int32 Cost;
		Vector2Int Pos;
	};

	GameZone();
	virtual ~GameZone();

	void Initialize();
	void Update();

	void Enter(SessionRef EnterSession);
	void Exit(SessionRef ExitSession);

	TSharedPtr<GameZone> GetZone() { return shared_from_this(); }
	TSharedPtr<AActor> FindActorBy(uint64 Id);

public:
	// PacketHandler
	void Incoming_C_Move(const Protocol::C_Move& Packet);

public:
	void AddActor(TSharedPtr<AActor> New);
	void RemoveActor(uint64 Id);
	void Broadcast(const TSharedPtr<SendBuffer>& SendBuffer);

public:
	// Client의 DevelopmentScene에 대응되는 개념이 Zone이므로, 여기에 로직 이관
	TSharedPtr<APlayer> FindNearestPlayerFrom(Vector2Int Cell);
	bool FindPath(Vector2Int Src, Vector2Int Dest, vector<Vector2Int>& OutPath, int32 MaxDepth = 10);
	bool CanMoveTo(Vector2Int Dest);
	bool GetRandomEmptyCell(Vector2Int& OutCell);
	TSharedPtr<APawn> GetPawnAt(Vector2Int Cell) const;

private:
	// 각각의 존은 ID로 플레이어들과 몬스터들을 구분함
	unordered_map<uint64, TSharedPtr<APlayer>> Players;
	unordered_map<uint64, TSharedPtr<AMonster>> Monsters;
	TileSet Map;
};

extern TSharedPtr<GameZone> GZone;
