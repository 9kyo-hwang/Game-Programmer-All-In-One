#pragma once
#include "AActor.h"

class Flipbook;

// Unity¿« Animator...? AnimatorController...?
class AFlipbook : public AActor
{
	using Super = AActor;

public:
	AFlipbook();
	~AFlipbook() override;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void Render(HDC DeviceContextHandle) override;

	void SetFlipbook(Flipbook* NewFlipbook);
	void Reset();

	bool HasAnimationFinished() const;

protected:
	Flipbook* CurrentFlipbook = nullptr;
	int32 Index = 0;
	float ElapsedTime = 0.f;
};
