#pragma once
#include "AActor.h"

class Tilemap;

class ATilemap : public AActor
{
	using Super = AActor;

public:
	ATilemap();
	~ATilemap() override;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void Render(HDC DeviceContextHandle) override;

	void PickTile();

	Tilemap* GetTilemap() const { return MyTilemap; }
	void SetTilemap(Tilemap* NewTilemap) { MyTilemap = NewTilemap; }

	void SetShowDebug(bool bShow) { bShowDebug = bShow; }

protected:
	Tilemap* MyTilemap = nullptr;
	bool bShowDebug = false;
};

