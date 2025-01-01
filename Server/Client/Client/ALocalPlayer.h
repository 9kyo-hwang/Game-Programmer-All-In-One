#pragma once
#include "APlayer.h"

class ALocalPlayer : public APlayer
{
	using Super = APlayer;

public:
	ALocalPlayer();
	~ALocalPlayer() override;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void Render(HDC DeviceContextHandle) override;

private:
	void Input();
	bool Move();

	void OnTickIdle(float DeltaTime) override;
	void OnTickMove(float DeltaTime) override;
	void OnTickAttack(float DeltaTime) override;

	void SynchronizeToServer() const;

private:
	bool bKeyPressed = false;
};
