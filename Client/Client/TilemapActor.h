#pragma once
#include "AActor.h"

class Tilemap;

class TilemapActor : public AActor
{
	using Super = AActor;

public:
	TilemapActor();
	~TilemapActor() override;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void Render(HDC DeviceContextHandle) override;

	void PickTile();

	Tilemap* GetTilemap() const { return MyTilemap; }
	void SetTilemap(Tilemap* NewTilemap) { MyTilemap = NewTilemap; }

	void SetShowDebug(bool bOption) { bShowDebug = bOption; }

protected:
	Tilemap* MyTilemap = nullptr;
	uint8 bShowDebug = false;
};

