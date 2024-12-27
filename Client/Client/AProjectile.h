#pragma once
#include "UObject.h"

class AProjectile : public UObject
{
	using Super = UObject;

public:
	AProjectile();
	~AProjectile() override;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void Render(HDC DeviceContextHandle) override;

protected:
	void OnTickIdle(float DeltaTime) override;
	void OnTickMove(float DeltaTime) override;
	void OnTickAttack(float DeltaTime) override;
	void UpdateAnimation() override;

protected:
	class APawn* Owner = nullptr;  // raw pointer 매우 위험!!!
};

