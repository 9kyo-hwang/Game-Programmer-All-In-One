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

	Vector GetSpeed() const { return Speed; }
	void SetSpeed(Vector NewSpeed) { Speed = NewSpeed; }

	Object* GetOwner() const { return Owner; }
	void SetOwner(Object* NewOwner) { Owner = NewOwner; }

private:
	Vector Speed{};
	Object* Owner = nullptr;
};

