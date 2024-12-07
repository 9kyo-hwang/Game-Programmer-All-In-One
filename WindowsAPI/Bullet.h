#pragma once
#include "Object.h"

class Bullet : public Object
{
	using Super = Object;

public:
	struct Stats
	{
		int32 Hp = 0;
		int32 MaxHp = 0;
		float Speed = 0.0f;
	};

	Bullet();
	~Bullet() override;

	void Initialize() override;
	void Update() override;
	void Render(HDC InDC) override;

public:
	void SetAngle(float NewAngle) { Angle = NewAngle; }

private:
	float Angle = 0.0f;
};

