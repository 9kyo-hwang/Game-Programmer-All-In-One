#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
	using Super = Collider;

public:
	SphereCollider();
	~SphereCollider() override;

	void BeginPlay() override;
	void TickComponent() override;
	void Render(HDC DeviceContextHandle) override;

	bool IsCollision(Collider* Other) override;

public:
	float Radius = 0.0f;
};

