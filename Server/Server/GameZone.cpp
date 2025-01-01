#include "pch.h"
#include "GameZone.h"

#include <ranges>

#include "AActor.h"
#include "APlayer.h"
#include "AMonster.h"
#include "GameSession.h"

// 보통은 Zone들을 관리할 Manager를 두지만, 우리는 심플하게 하나만...
TSharedPtr<GameZone> GZone = make_shared<GameZone>();

GameZone::GameZone()
{
}

GameZone::~GameZone()
{
}

void GameZone::Initialize()
{
	TSharedPtr<AMonster> Monster = AActor::SpawnActor<AMonster>();
	Monster->Info.set_posx(8);
	Monster->Info.set_posy(8);

	AddActor(Monster);

	Map.Load(L"E:\\Github\\Game-Programmer-All-In-One\\Server\\Client\\Resources\\Tilemap\\Tilemap01.txt");
}

void GameZone::Update()
{
	for (const auto& Player : Players | views::values)
	{
		Player->Update();
	}

	for (const auto& Monster : Monsters | views::values)
	{
		Monster->Update();
	}
}

void GameZone::Enter(SessionRef EnterSession)
{
	TSharedPtr<APlayer> NewPlayer = AActor::SpawnActor<APlayer>();

	// 세션 - 플레이어 연결
	EnterSession->Zone = GetZone();  // Shared -> Weak는 = 연산자로 바로 할당 가능
	EnterSession->Player = NewPlayer;
	NewPlayer->Session = EnterSession;
	NewPlayer->Info.set_posx(5);
	NewPlayer->Info.set_posy(5);

	// 아래 2가지는 새로운 패킷을 설계해서 구현

	// 새로 들어온 클라이언트에게 필요한 정보(자신이 누구인지) 넘겨주기
	{
		EnterSession->Send(ServerPacketHandler::Outgoing_S_LocalPlayer(NewPlayer->Info));  // 패킷 생성 후 반환받은 직렬화된 SendBuffer를 Session에 Send
	}

	// 주변 클라이언트들에게 새 플레이어가 입장했음을 알려줘야 함
	{
		Protocol::S_SpawnActor Packet;
		for (const auto& Player : Players | views::values)
		{
			*Packet.add_actors() = Player->Info;
		}

		for (const auto& Monster : Monsters | views::values)
		{
			*Packet.add_actors() = Monster->Info;
		}

		EnterSession->Send(ServerPacketHandler::Outgoing_S_SpawnActor(Packet));
	}

	AddActor(NewPlayer);
}

void GameZone::Exit(SessionRef ExitSession)
{
	if (ExitSession)
	{
		if (TSharedPtr<APlayer> Player = ExitSession->Player.lock())
		{
			RemoveActor(Player->Info.id());
		}
	}
}

TSharedPtr<AActor> GameZone::FindActorBy(uint64 Id)
{
	if (Players.contains(Id))
	{
		return Players[Id];
	}

	if (Monsters.contains(Id))
	{
		return Monsters[Id];
	}

	return nullptr;
}

void GameZone::Incoming_C_Move(const Protocol::C_Move& Packet)
{
	if (auto Actor = FindActorBy(Packet.info().id()))
	{
		// Check Validation(핵 방지)
		Actor->Info.set_state(Packet.info().state());
		Actor->Info.set_direction(Packet.info().direction());
		Actor->Info.set_posx(Packet.info().posx());
		Actor->Info.set_posy(Packet.info().posy());

		// Client의 변화를 적용했으므로 나머지 클라이언트들에게 S_Move 패킷 전달
		TSharedPtr<SendBuffer> Buffer = ServerPacketHandler::Outgoing_S_Move(Packet.info());
		Broadcast(Buffer);
	}
}

void GameZone::AddActor(TSharedPtr<AActor> New)
{
	uint64 Id = New->Info.id();
	switch (New->Info.type())
	{
	case Protocol::Player:
		Players[Id] = static_pointer_cast<APlayer>(New);
		break;
	case Protocol::Monster:
		Monsters[Id] = static_pointer_cast<AMonster>(New);
		break;
	default:
		return;
	}

	// TODO: 신규 액터 정보 전송
	New->EnteredZone = GetZone();

	Protocol::S_SpawnActor Packet;
	*Packet.add_actors() = New->Info;
	Broadcast(ServerPacketHandler::Outgoing_S_SpawnActor(Packet));
}

void GameZone::RemoveActor(uint64 Id)
{
	if (TSharedPtr<AActor> Actor = FindActorBy(Id))
	{
		switch (Actor->Info.type())
		{
		case Protocol::Player:
			Players.erase(Id);
			break;
		case Protocol::Monster:
			Monsters.erase(Id);
			break;
		default:
			return;
		}

		Actor->EnteredZone = nullptr;

		// TODO: 액터 삭제 알림
		Protocol::S_DestroyActor Packet;
		Packet.add_ids(Id);
		Broadcast(ServerPacketHandler::Outgoing_S_DestroyActor(Packet));
	}
}

void GameZone::Broadcast(const TSharedPtr<SendBuffer>& SendBuffer)
{
	for (const auto& Player : Players | views::values)
	{
		Player->Session->Send(SendBuffer);
	}
}

TSharedPtr<APlayer> GameZone::FindNearestPlayerFrom(Vector2Int Cell)
{
	float NearestDistance = FLT_MAX;
	TSharedPtr<APlayer> NearestPlayer = nullptr;

	// 여러 플레이어에 대응되는 방식
	for (const auto& Player : Players | views::values)
	{
		Vector2Int Direction = Cell - Player->GetCellPosition();
		float Distance = Direction.GetMagnitudeSquared();
		if (Distance < NearestDistance)
		{
			NearestDistance = Distance;
			NearestPlayer = Player;
		}
	}

	return NearestPlayer;
}

bool GameZone::FindPath(Vector2Int Src, Vector2Int Dest, vector<Vector2Int>& OutPath, int32 MaxDepth)
{
	// 너무 멀리 있는 적은 탐색 후보에서 제외
	int32 Depth = abs(Src.Y - Dest.Y) + abs(Src.X - Dest.X);
	if (Depth >= MaxDepth)
	{
		return false;
	}

	priority_queue<AStarNode, vector<AStarNode>, greater<AStarNode>> Queue;
	map<Vector2Int, int32> BestCost;
	map<Vector2Int, Vector2Int> PrevPos;

	// 초기값
	{
		int32 Distance = abs(Dest.X - Src.X) + abs(Dest.Y - Src.Y);
		Queue.emplace(Distance, Src);
		BestCost[Src] = Distance;
		PrevPos[Src] = Src;
	}

	const Vector2Int Offset[4]
	{
		{0, -1},
		{0, 1},
		{-1, 0},
		{1, 0}
	};

	bool bFound = false;
	while (!Queue.empty())
	{
		AStarNode Current = Queue.top(); Queue.pop();

		// 기존에 알고 있는 비용이 더 좋다면 skip
		if (BestCost[Current.Pos] < Current.Cost)
		{
			continue;
		}

		if (Current.Pos == Dest)
		{
			bFound = true;
			break;
		}

		for (const Vector2Int& Front : Offset)
		{
			Vector2Int NextPos = Current.Pos + Front;
			if (!CanMoveTo(NextPos))
			{
				continue;
			}

			Depth = abs(Src.Y - NextPos.Y) + abs(Src.X - NextPos.X);
			if (Depth >= MaxDepth)
			{
				continue;
			}

			int32 NewCost = abs(Dest.X - NextPos.X) + abs(Dest.Y - NextPos.Y);
			if (BestCost.contains(NextPos) && BestCost[NextPos] <= NewCost)
			{
				continue;
			}

			BestCost[NextPos] = NewCost;
			Queue.emplace(NewCost, NextPos);
			PrevPos[NextPos] = Current.Pos;
		}
	}

	if (!bFound)
	{
		// 맨 마지막 탐색때 플레이어의 존재로 인해 CanMoveTo가 false를 반환. 이에 대한 핸들링
		float BestScore = FLT_MAX;
		for (const auto& [Pos, Score] : BestCost)
		{
			// 동점의 경우 가장 덜 이동한 쪽으로
			if (BestScore == Score)
			{
				int32 Src2Dest = abs(Dest.X - Src.X) + abs(Dest.Y - Src.Y);
				int32 Src2Cur = abs(Pos.X - Src.X) + abs(Pos.Y - Src.Y);

				if (Src2Dest > Src2Cur)
				{
					Dest = Pos;
				}
			}
			else if (BestScore > Score)
			{
				Dest = Pos;
				BestScore = Score;
			}
		}
	}

	OutPath.clear();
	Vector2Int Current = Dest;
	while (true)
	{
		OutPath.push_back(Current);
		if (Current == PrevPos[Current])
		{
			break;
		}

		Current = PrevPos[Current];
	}

	ranges::reverse(OutPath);
	return true;
}

bool GameZone::CanMoveTo(Vector2Int Dest)
{
	// 서버에서도 지형 정보가 필요함
	Tile* DestTile = Map.GetTileAt(Dest);
	if (!DestTile)
	{
		return false;
	}

	if (GetPawnAt(Dest))
	{
		return false;
	}

	return DestTile->Value != 1;
}

bool GameZone::GetRandomEmptyCell(Vector2Int& OutCell)
{
	Vector2Int MapSize = Map.GetSize();
	while (true)
	{
		int32 X = rand() % MapSize.X;
		int32 Y = rand() % MapSize.Y;

		// 모든 칸이 이동 불가라면...? -> loop 횟수에 제한을 두는 것도...
		if (CanMoveTo({ X, Y }))
		{
			OutCell = { X, Y };
			return true;
		}
	}

	return false;
}

TSharedPtr<APawn> GameZone::GetPawnAt(Vector2Int Cell) const
{
	for (const auto& Player : Players | views::values)
	{
		if (Player->GetCellPosition() == Cell)
		{
			return Player;
		}
	}

	for (const auto& Monster : Monsters | views::values)
	{
		if (Monster->GetCellPosition() == Cell)
		{
			return Monster;
		}
	}

	return nullptr;
}
