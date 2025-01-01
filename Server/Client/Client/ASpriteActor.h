#pragma once
#include "AActor.h"

class Sprite;

class ASprite : public AActor
{
	using Super = AActor;

public:
	ASprite();
	~ASprite() override;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void Render(HDC InDC) override;

	void SetSprite(Sprite* NewSprite) { MySprite = NewSprite; }

protected:
	Sprite* MySprite = nullptr;
};

