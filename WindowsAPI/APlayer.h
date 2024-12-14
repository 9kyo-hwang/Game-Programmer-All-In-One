#pragma once
#include "FlipbookActor.h"

class BoxCollider;

class APlayer : public FlipbookActor
{
	using Super = FlipbookActor;

public:
	APlayer();
	~APlayer() override;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void Render(HDC DeviceContextHandle) override;

	void OnComponentBeginOverlap(Collider* This, Collider* Other) override;
	void OnComponentEndOverlap(Collider* This, Collider* Other) override;

private:
	void Jump();
	void OnTickGravity(float DeltaTime);
	void AdjustCollisionPos(BoxCollider* This, BoxCollider* Other);

private:
	Flipbook* FB_MoveUp = nullptr;
	Flipbook* FB_MoveDown = nullptr;
	Flipbook* FB_MoveLeft = nullptr;
	Flipbook* FB_MoveRight = nullptr;

	Vector2 Speed{};
	int32 Gravity = 1000;
	uint8 bOnGround = false;
	uint8 bJumping = false;
};

