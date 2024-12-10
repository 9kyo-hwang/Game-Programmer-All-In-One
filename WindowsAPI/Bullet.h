#pragma once
#include "Object.h"

class Bullet final : public Object
{
	using Super = Object;

public:
	Bullet();
	~Bullet() override;

	void Initialize() override;
	void Update() override;
	void Render(HDC InDC) override;

	void SetSpeed(Vector NewSpeed) { Speed = NewSpeed; }

private:
	Vector Speed{};
};

