#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
	using Super = Collider;

public:
	BoxCollider();
	~BoxCollider() override;

	void BeginPlay() override;
	void TickComponent() override;
	void Render(HDC DeviceContextHandle) override;

	bool IsCollision(Collider* Other) override;

public:
	Vector2 Size{};
};

