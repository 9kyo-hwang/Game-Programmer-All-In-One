#pragma once
#include "Scene.h"

class Texture;
class Sound;
class UObject;

class DevelopmentScene final : public Scene
{
	using Super = Scene;

public:
	DevelopmentScene();
	~DevelopmentScene() override;

	void Initialize() override;
	void Update(float DeltaTime) override;  // ÇÊ¿ä¿¡ µû¶ó FixedUpdate¸¦ µÖµµ µÊ
	void Render(HDC DeviceContextHandle) override;

	void AddActor(AActor* NewActor) override;
	void RemoveActor(AActor* TargetActor) override;

	bool CanMoveTo(Vector2Int Dest) const;
	Vector2 CellToWorld(Vector2Int Cell) const;
	bool GetRandomEmptyCell(Vector2Int& OutCell) const;

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
		AddActor(Object);

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
	const int32 DesiredMonsterCount = 40;
	int32 CurrentMonsterCount = 0;

	class ATilemap* ActiveTilemapActor = nullptr;
};
