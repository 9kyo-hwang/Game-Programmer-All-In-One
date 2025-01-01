#pragma once
#include "APawn.h"

class AMonster : public APawn
{
	using Super = APawn;

public:
	AMonster();
	~AMonster() override;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void Render(HDC DeviceContextHandle) override;

protected:
	void OnTickIdle(float DeltaTime) override;
	void OnTickMove(float DeltaTime) override;
	void OnTickAttack(float DeltaTime) override;
	void UpdateAnimation() override;

protected:
	// TODO
	Flipbook* MoveFlipbooks[4]{};
	class APlayer* Target = nullptr;  // �ſ� ����! smart_pointer�� �ٲٴ��� ID�� �߱��϶�!
	float WaitSecs = 0.f;
};

