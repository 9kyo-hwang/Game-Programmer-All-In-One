#pragma once
#include "Scene.h"

class Texture;
class Sound;
class UObject;
class APlayer;

class DevelopmentScene final : public Scene
{
	using Super = Scene;

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

	DevelopmentScene();
	~DevelopmentScene() override;

	void Initialize() override;
	void Update(float DeltaTime) override;  // ÇÊ¿ä¿¡ µû¶ó FixedUpdate¸¦ µÖµµ µÊ
	void Render(HDC DeviceContextHandle) override;

	void SpawnActor(AActor* Target) override;
	void DestroyActor(AActor* Target) override;

	bool CanMoveTo(Vector2Int Dest) const;
	Vector2 CellToWorld(Vector2Int Cell) const;
	bool GetRandomEmptyCell(Vector2Int& OutCell) const;

	APlayer* FindNearestPlayerFrom(Vector2Int Cell);
	bool FindPath(Vector2Int Src, Vector2Int Dest, vector<Vector2Int>& OutPath, int32 MaxDepth = 10) const;

	template<class ObjectType> requires std::is_base_of_v<UObject, ObjectType>
	ObjectType* NewObject()
	{
		Vector2Int Cell{ -1, -1 };
		if (GetRandomEmptyCell(Cell))
		{
			return NewObject<ObjectType>(Cell);
		}

		assert(false && "DevelopmentScene::NewObject Failed");
		return nullptr;
	}

	template<typename ObjectType> requires std::is_base_of_v<UObject, ObjectType>
	ObjectType* NewObject(Vector2Int Position)
	{
		ObjectType* Object = new ObjectType();
		Object->MoveTo(Position, true);
		SpawnActor(Object);

		Object->BeginPlay();

		return Object;
	}

private:
	void LoadMap(Texture* Stage01Texture);
	void LoadPlayer(Texture* PlayerUp, Texture* PlayerDown, Texture* PlayerLeft, Texture* PlayerRight);
	void LoadMonster(Texture* Snake);
	void LoadProjectile(Texture* Arrow);
	void LoadEffect(Texture* Hit);
	void LoadTilemap();
	void LoadUI(Texture* Start, Texture* Edit, Texture* Exit);
	void LoadSound(Sound* BGM, Sound* SwordFx);

	void SpawnMonster();

private:
	const int32 DesiredMonsterCount = 20;
	int32 CurrentMonsterCount = 0;

	class ATilemap* ActiveTilemapActor = nullptr;
};
