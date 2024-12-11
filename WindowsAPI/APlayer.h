#pragma once
#include "FlipbookActor.h"

class APlayer : public FlipbookActor
{
	using Super = FlipbookActor;

public:
	APlayer();
	~APlayer() override;

	void BeginPlay() override;
	void Tick() override;
	void Render(HDC DeviceContextHandle) override;

	void OnComponentBeginOverlap(Collider* This, Collider* Other) override;
	void OnComponentEndOverlap(Collider* This, Collider* Other) override;

private:
	Flipbook* FB_MoveUp = nullptr;
	Flipbook* FB_MoveDown = nullptr;
	Flipbook* FB_MoveLeft = nullptr;
	Flipbook* FB_MoveRight = nullptr;
};

