#pragma once
#include "Object.h"

class Player : public Object
{
	using Super = Object;

public:
	Player();
	virtual ~Player() override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC InDC) override;

	Vector2 GetFirePosition();

private:
	float CannonAngle = 0.0f;
	float CannonLength = 100.0f;
};

