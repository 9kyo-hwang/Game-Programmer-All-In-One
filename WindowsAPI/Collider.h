#pragma once
#include "Component.h"

enum class EColliderType
{
	Box,
	Sphere,
};

class BoxCollider;
class SphereCollider;

class Collider : public Component
{
	using Super = Component;

public:
	Collider(EColliderType InType);
	~Collider() override;

	void BeginPlay() override;
	void TickComponent() override;
	void Render(HDC DeviceContextHandle) override;

	virtual bool IsCollision(Collider* Other);

	EColliderType GetColliderType() const { return Type; }
	void SetShowDebug(bool bOption) { bShowDebug = bOption; }

public:
	static bool OnCollision(BoxCollider* Box1, BoxCollider* Box2);
	static bool OnCollision(SphereCollider* Sphere, BoxCollider* Box);
	static bool OnCollision(SphereCollider* Sphere1, SphereCollider* Sphere2);

protected:
	EColliderType Type;
	uint8 bShowDebug = true;

public:  // �ӽ÷� public
	// Overlap Begin ~ End ���̿�, ��ģ ������ ����ϱ� ���� �����
	unordered_set<Collider*> CollidedActorsSet;
	// �浹 ���踦 �����ϱ� ���� ��Ʈ �÷���
};

