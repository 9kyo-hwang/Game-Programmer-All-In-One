#pragma once
#include "AActor.h"

class Flipbook;

// Unity¿« Animator...? AnimatorController...?
class FlipbookActor : public AActor
{
	using Super = AActor;

public:
	FlipbookActor();
	~FlipbookActor() override;

	void BeginPlay() override;
	void Tick() override;
	void Render(HDC InDC) override;

	void SetFlipbook(Flipbook* NewFlipbook);
	void Reset();

protected:
	Flipbook* MyFlipbook = nullptr;
	int32 Index = 0;
	float ElapsedTime = 0.f;
};
