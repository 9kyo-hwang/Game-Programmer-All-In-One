#pragma once
#include "APawn.h"

class APlayer : public APawn
{
	using Super = APawn;

public:
	APlayer();
	~APlayer() override;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void Render(HDC DeviceContextHandle) override;

protected:
	void OnTickIdle(float DeltaTime) override;
	void OnTickMove(float DeltaTime) override;
	void OnTickAttack(float DeltaTime) override;
	void UpdateAnimation() override;

protected:
	vector<vector<Flipbook*>> Flipbooks{};
	bool bKeyPressed = false;
	static const Vector2Int Offset[4];
};

