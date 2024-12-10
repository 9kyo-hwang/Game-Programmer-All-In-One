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
	void Render(HDC InDC) override;

private:
	Flipbook* FB_MoveUp = nullptr;
	Flipbook* FB_MoveDown = nullptr;
	Flipbook* FB_MoveLeft = nullptr;
	Flipbook* FB_MoveRight = nullptr;
};

