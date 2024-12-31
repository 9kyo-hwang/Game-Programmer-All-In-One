#include "pch.h"
#include "DevelopmentScene.h"

#include <queue>

#include "ALocalPlayer.h"
#include "AMonster.h"
#include "ASpriteActor.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "APlayer.h"
#include "Flipbook.h"
#include "SceneManager.h"
#include "Tilemap.h"
#include "TilemapActor.h"
#include "Sound.h"

DevelopmentScene::DevelopmentScene()
{
}

DevelopmentScene::~DevelopmentScene()
{
	
}

void DevelopmentScene::Initialize()
{
	Texture* Tile = ResourceManager::Get()->LoadTexture(L"Tile", L"Sprite\\Map\\Tile.bmp", RGB(128, 128, 128));
	ResourceManager::Get()->CreateSprite(L"Tile_O", Tile, 0, 0, 48, 48);
	ResourceManager::Get()->CreateSprite(L"Tile_X", Tile, 48, 0, 48, 48);

	LoadMap(
		ResourceManager::Get()->LoadTexture(L"Stage01", L"Sprite\\Map\\Stage01.bmp")
	);

	LoadTilemap();

	LoadUI(
		ResourceManager::Get()->LoadTexture(L"Start", L"Sprite\\UI\\Start.bmp"),
		ResourceManager::Get()->LoadTexture(L"Edit", L"Sprite\\UI\\Edit.bmp"),
		ResourceManager::Get()->LoadTexture(L"Exit", L"Sprite\\UI\\Exit.bmp")
	);

	LoadPlayer(
		ResourceManager::Get()->LoadTexture(L"PlayerUp", L"Sprite\\Player\\PlayerUp.bmp", RGB(128, 128, 128)),
		ResourceManager::Get()->LoadTexture(L"PlayerDown", L"Sprite\\Player\\PlayerDown.bmp", RGB(128, 128, 128)),
		ResourceManager::Get()->LoadTexture(L"PlayerLeft", L"Sprite\\Player\\PlayerLeft.bmp", RGB(128, 128, 128)),
		ResourceManager::Get()->LoadTexture(L"PlayerRight", L"Sprite\\Player\\PlayerRight.bmp", RGB(128, 128, 128))
	);

	LoadMonster(
		ResourceManager::Get()->LoadTexture(L"Snake", L"Sprite\\Monster\\Snake.bmp", RGB(128, 128, 128))
	);

	LoadProjectile(
		ResourceManager::Get()->LoadTexture(L"Arrow", L"Sprite\\Item\\Arrow.bmp", RGB(128, 128, 128))
	);

	LoadEffect(
		ResourceManager::Get()->LoadTexture(L"Hit", L"Sprite\\Effect\\Hit.bmp", RGB(0, 0, 0))
	);

	LoadSound(
		ResourceManager::Get()->LoadSound(L"BGM", L"Sound\\BGM.wav"),
		ResourceManager::Get()->LoadSound(L"Attack", L"Sound\\Sword.wav")
	);

	//TODO: 이제 서버 GameZone에서 지정해준 정보를 토대로, LocalPlayer 정보를 받아 생성
	//NewObject<ALocalPlayer>({ 5, 5 });
	//NewObject<AMonster>({ 7, 7 });

	Super::Initialize();
}

void DevelopmentScene::Update(float DeltaTime)
{
	Super::Update();

	SpawnMonster();
}

void DevelopmentScene::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

// Monster Count를 관리하기 위해 Add/Remove를 virtual - override
void DevelopmentScene::SpawnActor(AActor* Target)
{
	Super::SpawnActor(Target);
	if (AMonster* Monster = dynamic_cast<AMonster*>(Target))
	{
		CurrentMonsterCount++;
	}
}

void DevelopmentScene::DestroyActor(AActor* Target)
{
	Super::DestroyActor(Target);
	if (AMonster* Monster = dynamic_cast<AMonster*>(Target))
	{
		CurrentMonsterCount--;
	}
}

bool DevelopmentScene::CanMoveTo(Vector2Int Dest) const
{
	if (!ActiveTilemapActor)
	{
		return false;
	}

	Tilemap* ActiveTilemap = ActiveTilemapActor->GetTilemap();
	if (!ActiveTilemap)
	{
		return false;
	}

	Tile* DestTile = ActiveTilemap->GetTileAt(Dest);
	if (!DestTile)
	{
		return false;
	}

	if (GetPawnAt(Dest))
	{
		return false;
	}

	return DestTile->Value != 1;

	// 추후 몬스터가 추가되면 여기서 추가로 몬스터 위치를 확인
}

Vector2 DevelopmentScene::CellToWorld(Vector2Int Cell) const
{
	assert(ActiveTilemapActor != nullptr);

	Tilemap* ActiveTilemap = ActiveTilemapActor->GetTilemap();
	assert(ActiveTilemap != nullptr);

	int32 NumTile = ActiveTilemap->GetNumTile();
	Vector2 TilemapPosition = ActiveTilemapActor->GetCurrentPosition();

	return
	{
		TilemapPosition.X + Cell.X * NumTile + NumTile / 2,
		TilemapPosition.Y + Cell.Y * NumTile + NumTile / 2
	};
}

bool DevelopmentScene::GetRandomEmptyCell(Vector2Int& OutCell) const
{
	if (!ActiveTilemapActor)
	{
		return false;
	}

	Tilemap* ActiveTilemap = ActiveTilemapActor->GetTilemap();
	if (!ActiveTilemap)
	{
		return false;
	}

	Vector2Int MapSize = ActiveTilemap->GetMapSize();
	while (true)
	{
		int32 X = rand() % MapSize.X;
		int32 Y = rand() % MapSize.Y;

		// 모든 칸이 이동 불가라면...? -> loop 횟수에 제한을 두는 것도...
		if (CanMoveTo({X, Y}))
		{
			OutCell = { X, Y };
			return true;
		}
	}

	return false;
}

APlayer* DevelopmentScene::FindNearestPlayerFrom(Vector2Int Cell)
{
	float NearestDistance = FLT_MAX;
	APlayer* NearestPlayer = nullptr;

	// 여러 플레이어에 대응되는 방식
	for (AActor* Actor : Actors[static_cast<int32>(ERenderLayer::Object)])
	{
		if (APlayer* Player = dynamic_cast<APlayer*>(Actor))
		{
			Vector2Int Direction = Cell - Player->GetCellPosition();
			float Distance = Direction.GetMagnitudeSquared();
			if (Distance < NearestDistance)
			{
				NearestDistance = Distance;
				NearestPlayer = Player;
			}
		}
	}

	return NearestPlayer;
}

// F = G(Start -> Point) + H(Point -> Dest)
bool DevelopmentScene::FindPath(Vector2Int Src, Vector2Int Dest, vector<Vector2Int>& OutPath, int32 MaxDepth) const
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

UObject* DevelopmentScene::FindObjectBy(uint64 Id) const
{
	for (AActor* Actor : Actors[static_cast<int32>(ERenderLayer::Object)])
	{
		if (UObject* Object = dynamic_cast<UObject*>(Actor))
		{
			if (Object->Info.id() == Id)
			{
				return Object;
			}
		}
	}

	return nullptr;
}

void DevelopmentScene::Handle_S_SpawnActor(const Protocol::S_SpawnActor& Packet)
{
	const uint64 LocalPlayerId = SceneManager::Get()->GetLocalPlayerId();

	for (const auto& Info : Packet.actors())
	{
		if (LocalPlayerId == Info.id())
		{
			continue;
		}

		if (Info.type() == Protocol::EObjectType::Player)
		{
			APlayer* Player = NewObject<APlayer>(Vector2Int{ Info.posx(), Info.posy() });
			Player->RotateTo(Info.direction());  // 애니메이션 갱신을 위해
			Player->TransitionTo(Info.state());  // 이들을 호출
			Player->Info = Info;
		}
		else if (Info.type() == Protocol::EObjectType::Monster)
		{
			AMonster* Monster = NewObject<AMonster>(Vector2Int{ Info.posx(), Info.posy() });
			Monster->RotateTo(Info.direction());  // 애니메이션 갱신을 위해
			Monster->TransitionTo(Info.state());  // 이들을 호출
			Monster->Info = Info;
		}
	}
}

void DevelopmentScene::Handle_S_DestroyActor(const Protocol::S_DestroyActor& Packet)
{
	for (uint64 Id : Packet.ids())
	{
		if (UObject* Object = FindObjectBy(Id))
		{
			DestroyActor(Object);
		}
	}
}

void DevelopmentScene::LoadMap(Texture* Stage01Texture)
{
	assert(nullptr != Stage01Texture);
	{
		Sprite* Stage01Sprite = ResourceManager::Get()->CreateSprite(L"Stage01", Stage01Texture);

		ASprite* Stage01 = new ASprite();
		Stage01->SetSprite(Stage01Sprite);
		Stage01->SetLayer(ERenderLayer::Background);

		const Vector2Int StageSize = Stage01Sprite->GetSize();
		Stage01->SetCurrentPosition(Vector2(StageSize.X / 2, StageSize.Y / 2));

		SpawnActor(Stage01);
	}
}

void DevelopmentScene::LoadPlayer(Texture* PlayerUp, Texture* PlayerDown, Texture* PlayerLeft, Texture* PlayerRight)
{
	assert(nullptr != PlayerUp);
	assert(nullptr != PlayerDown);
	assert(nullptr != PlayerLeft);
	assert(nullptr != PlayerDown);

	ResourceManager::Get()->LoadTexture(L"Sword", L"Sprite\\Item\\Sword.bmp");
	ResourceManager::Get()->LoadTexture(L"Bow", L"Sprite\\Item\\Bow.bmp");
	ResourceManager::Get()->LoadTexture(L"Staff", L"Sprite\\Item\\Staff.bmp");

	// PlayerIdle
	{
		Flipbook* FB_Up = ResourceManager::Get()->CreateFlipbook(L"FB_IdleUp");
		Flipbook* FB_Down = ResourceManager::Get()->CreateFlipbook(L"FB_IdleDown");
		Flipbook* FB_Left = ResourceManager::Get()->CreateFlipbook(L"FB_IdleLeft");
		Flipbook* FB_Right = ResourceManager::Get()->CreateFlipbook(L"FB_IdleRight");

		FB_Up->SetInfo({ PlayerUp, L"FB_IdleUp", {200, 200}, 0, 9, 0, 0.5f, true });
		FB_Down->SetInfo({ PlayerDown, L"FB_IdleDown", {200, 200}, 0, 9, 0, 0.5f, true });
		FB_Left->SetInfo({ PlayerLeft, L"FB_IdleLeft", {200, 200}, 0, 9, 0, 0.5f, true });
		FB_Right->SetInfo({ PlayerRight, L"FB_IdleRight", {200, 200}, 0, 9, 0, 0.5f, true });
	}
	// PlayerMove
	{
		Flipbook* FB_Up = ResourceManager::Get()->CreateFlipbook(L"FB_MoveUp");
		Flipbook* FB_Down = ResourceManager::Get()->CreateFlipbook(L"FB_MoveDown");
		Flipbook* FB_Left = ResourceManager::Get()->CreateFlipbook(L"FB_MoveLeft");
		Flipbook* FB_Right = ResourceManager::Get()->CreateFlipbook(L"FB_MoveRight");

		FB_Up->SetInfo({ PlayerUp, L"FB_MoveUp", {200, 200}, 0, 9, 1, 0.5f, true });
		FB_Down->SetInfo({ PlayerDown, L"FB_MoveDown", {200, 200}, 0, 9, 1, 0.5f, true });
		FB_Left->SetInfo({ PlayerLeft, L"FB_MoveLeft", {200, 200}, 0, 9, 1, 0.5f, true });
		FB_Right->SetInfo({ PlayerRight, L"FB_MoveRight", {200, 200}, 0, 9, 1, 0.5f, true });
	}
	// PlayerAttack
	{
		Flipbook* FB_Up = ResourceManager::Get()->CreateFlipbook(L"FB_AttackUp");
		Flipbook* FB_Down = ResourceManager::Get()->CreateFlipbook(L"FB_AttackDown");
		Flipbook* FB_Left = ResourceManager::Get()->CreateFlipbook(L"FB_AttackLeft");
		Flipbook* FB_Right = ResourceManager::Get()->CreateFlipbook(L"FB_AttackRight");

		FB_Up->SetInfo({ PlayerUp, L"FB_AttackUp", {200, 200}, 0, 7, 3, 0.5f, false });
		FB_Down->SetInfo({ PlayerDown, L"FB_AttackDown", {200, 200}, 0, 7, 3, 0.5f, false});
		FB_Left->SetInfo({ PlayerLeft, L"FB_AttackLeft", {200, 200}, 0, 7, 3, 0.5f, false });
		FB_Right->SetInfo({ PlayerRight, L"FB_AttackRight", {200, 200}, 0, 7, 3, 0.5f, false });
	}
	// PlayerBow
	{
		Flipbook* FB_Up = ResourceManager::Get()->CreateFlipbook(L"FB_BowUp");
		Flipbook* FB_Down = ResourceManager::Get()->CreateFlipbook(L"FB_BowDown");
		Flipbook* FB_Left = ResourceManager::Get()->CreateFlipbook(L"FB_BowLeft");
		Flipbook* FB_Right = ResourceManager::Get()->CreateFlipbook(L"FB_BowRight");

		FB_Up->SetInfo({ PlayerUp, L"FB_BowUp", {200, 200}, 0, 7, 5, 0.5f, false });
		FB_Down->SetInfo({ PlayerDown, L"FB_BowDown", {200, 200}, 0, 7, 5, 0.5f, false });
		FB_Left->SetInfo({ PlayerLeft, L"FB_BowLeft", {200, 200}, 0, 7, 5, 0.5f, false });
		FB_Right->SetInfo({ PlayerRight, L"FB_BowRight", {200, 200}, 0, 7, 5, 0.5f, false });
	}
	// PlayerStaff
	{
		Flipbook* FB_Up = ResourceManager::Get()->CreateFlipbook(L"FB_StaffUp");
		Flipbook* FB_Down = ResourceManager::Get()->CreateFlipbook(L"FB_StaffDown");
		Flipbook* FB_Left = ResourceManager::Get()->CreateFlipbook(L"FB_StaffLeft");
		Flipbook* FB_Right = ResourceManager::Get()->CreateFlipbook(L"FB_StaffRight");

		FB_Up->SetInfo({ PlayerUp, L"FB_StaffUp", {200, 200}, 0, 10, 6, 0.5f, false });
		FB_Down->SetInfo({ PlayerDown, L"FB_StaffDown", {200, 200}, 0, 10, 6, 0.5f, false });
		FB_Left->SetInfo({ PlayerLeft, L"FB_StaffLeft", {200, 200}, 0, 10, 6, 0.5f, false });
		FB_Right->SetInfo({ PlayerRight, L"FB_StaffRight", {200, 200}, 0, 10, 6, 0.5f, false });
	}
}

void DevelopmentScene::LoadMonster(Texture* Snake)
{
	assert(nullptr != Snake);
	// Snake Move
	{
		Flipbook* FB_Up = ResourceManager::Get()->CreateFlipbook(L"FB_SnakeUp");
		Flipbook* FB_Down = ResourceManager::Get()->CreateFlipbook(L"FB_SnakeDown");
		Flipbook* FB_Left = ResourceManager::Get()->CreateFlipbook(L"FB_SnakeLeft");
		Flipbook* FB_Right = ResourceManager::Get()->CreateFlipbook(L"FB_SnakeRight");

		FB_Up->SetInfo({ Snake, L"FB_SnakeUp", {100, 100}, 0, 3, 3, 0.5f });
		FB_Down->SetInfo({ Snake, L"FB_SnakeDown", {100, 100}, 0, 3, 0, 0.5f });
		FB_Left->SetInfo({ Snake, L"FB_SnakeLeft", {100, 100}, 0, 3, 2, 0.5f });
		FB_Right->SetInfo({ Snake, L"FB_SnakeRight", {100, 100}, 0, 3, 1, 0.5f });
	}
}

void DevelopmentScene::LoadProjectile(Texture* Arrow)
{
	assert(nullptr != Arrow);
	// Arrow Move
	{
		Flipbook* FB_Up = ResourceManager::Get()->CreateFlipbook(L"FB_ArrowUp");
		Flipbook* FB_Down = ResourceManager::Get()->CreateFlipbook(L"FB_ArrowDown");
		Flipbook* FB_Left = ResourceManager::Get()->CreateFlipbook(L"FB_ArrowLeft");
		Flipbook* FB_Right = ResourceManager::Get()->CreateFlipbook(L"FB_ArrowRight");

		FB_Up->SetInfo({ Arrow, L"FB_ArrowUp", {100, 100}, 0, 0, 3, 0.5f });
		FB_Down->SetInfo({ Arrow, L"FB_ArrowDown", {100, 100}, 0, 0, 0, 0.5f });
		FB_Left->SetInfo({ Arrow, L"FB_ArrowLeft", {100, 100}, 0, 0, 1, 0.5f });
		FB_Right->SetInfo({ Arrow, L"FB_ArrowRight", {100, 100}, 0, 0, 2, 0.5f });
	}
}

void DevelopmentScene::LoadEffect(Texture* Hit)
{
	assert(nullptr != Hit);
	{
		Flipbook* FB_Hit = ResourceManager::Get()->CreateFlipbook(L"FB_Hit");
		FB_Hit->SetInfo({ Hit, L"FB_Hit", {50, 47}, 0, 5, 0, 0.5f, false });
	}
}

void DevelopmentScene::LoadTilemap()
{
	ATilemap* NewTilemapActor = new ATilemap();
	SpawnActor(NewTilemapActor);
	ActiveTilemapActor = NewTilemapActor;
	{
		Tilemap* Tilemap01 = ResourceManager::Get()->CreateTilemap(L"Tilemap_01");
		Tilemap01->SetMapSize({ 63, 43 });
		Tilemap01->SetTileSize(48);

		ResourceManager::Get()->LoadTilemap(L"Tilemap_01", L"Tilemap\\Tilemap01.txt");

		ActiveTilemapActor->SetTilemap(Tilemap01);
		ActiveTilemapActor->SetShowDebug(false);
	}
}

void DevelopmentScene::LoadUI(Texture* Start, Texture* Edit, Texture* Exit)
{
	ResourceManager::Get()->CreateSprite(L"Start_Off", Start, 0, 0, 150, 150);
	ResourceManager::Get()->CreateSprite(L"Start_On", Start, 150, 0, 150, 150);
	ResourceManager::Get()->CreateSprite(L"Edit_Off", Edit, 0, 0, 150, 150);
	ResourceManager::Get()->CreateSprite(L"Edit_On", Edit, 150, 0, 150, 150);
	ResourceManager::Get()->CreateSprite(L"Exit_Off", Exit, 0, 0, 150, 150);
	ResourceManager::Get()->CreateSprite(L"Exit_On", Exit, 150, 0, 150, 150);
}

void DevelopmentScene::LoadSound(Sound* BGM, Sound* SwordFx)
{
	// 현재 방식은 하나의 리소스를 돌려쓰는 방식
	// A에서 재생시킨 BGM이 다 끝나지 않았을 때 B가 BGM을 다시 재생하면 중간에 강제로 끊고 다시 재생
	// FMOD 같은 라이브러리는 이를 해결하기 위해 "채널링" 개념 도입

	//BGM->Play(true);
}

// 일종의 몬스터 SpawningPool
void DevelopmentScene::SpawnMonster()
{
	if (CurrentMonsterCount < DesiredMonsterCount)
	{
		NewObject<AMonster>();
	}
}

