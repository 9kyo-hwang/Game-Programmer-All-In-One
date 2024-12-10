#pragma once
#include "ASpriteActor.h"

class APlayer : public ASpriteActor
{
	using Super = ASpriteActor;

public:
	APlayer();
	~APlayer() override;

	void BeginPlay() override;
	void Tick() override;
	void Render(HDC InDC) override;
};

