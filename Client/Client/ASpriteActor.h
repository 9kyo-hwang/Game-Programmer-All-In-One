#pragma once
#include "AActor.h"

class Sprite;

class ASpriteActor : public AActor
{
	using Super = AActor;

public:
	ASpriteActor();
	~ASpriteActor() override;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void Render(HDC InDC) override;

	void SetSprite(Sprite* NewSprite) { MySprite = NewSprite; }

protected:
	Sprite* MySprite = nullptr;
};

