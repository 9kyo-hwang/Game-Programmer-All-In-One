#pragma once
#include "Component.h"

class Sprite;

class SpriteRenderer : public Component
{
public:
	void Start() override;
	void Update() override;
	void Render(HDC InDC) override;

	void SetSprite(Sprite* NewSprite) { MySprite = NewSprite; }

private:
	Sprite* MySprite = nullptr;
};

