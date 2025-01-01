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

	EWeapons GetCurrentWeapon() const { return CurrentWeapon; }
	void ChangeWeapon(EWeapons NewWeapon) { CurrentWeapon = NewWeapon; }

private:
	Flipbook* IdleFlipbooks[4]{};
	Flipbook* MoveFlipbooks[4]{};
	Flipbook* SwordFlipbooks[4]{};
	Flipbook* BowFlipbooks[4]{};
	Flipbook* StaffFlipbooks[4]{};

	bool bKeyPressed = false;
	EWeapons CurrentWeapon = EWeapons::Sword;
};

