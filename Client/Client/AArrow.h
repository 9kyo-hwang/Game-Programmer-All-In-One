#pragma once
#include "AProjectile.h"

class AArrow : public AProjectile
{
	using Super = AProjectile;

public:
	AArrow();
	~AArrow() override;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void Render(HDC DeviceContextHandle) override;

protected:
	void OnTickIdle(float DeltaTime) override;
	void OnTickMove(float DeltaTime) override;
	void UpdateAnimation() override;

private:
	Flipbook* MoveFlipbooks[4]{};
};

