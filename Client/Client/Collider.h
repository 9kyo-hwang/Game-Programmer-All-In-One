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

	ECollideLayer GetCollideLayer() const { return Layer; }
	void SetCollideLayer(ECollideLayer NewLayer) { Layer = NewLayer; }

	uint32 GetCollisionFlag() const { return CollisionFlag; }
	void SetCollisionFlag(uint32 Flag) { CollisionFlag = Flag; }

	void ResetCollisionFlag() { CollisionFlag = 0; }
	void AddCollisionFlagLayer(ECollideLayer InLayer);
	void RemoveCollisionFlagLayer(ECollideLayer InLayer);

protected:
	EColliderType Type;
	uint8 bShowDebug = true;

public:  // �ӽ÷� public
	// Overlap Begin ~ End ���̿�, ��ģ ������ ����ϱ� ���� �����
	unordered_set<Collider*> CollidedActorsSet;

	// ���� �����ΰ�? ������ �浹�� ���ΰ�?
	ECollideLayer Layer = ECollideLayer::Object;
	// �浹 ���踦 �����ϱ� ���� ��Ʈ �÷���
	uint32 CollisionFlag = 0;  // ���ʴ� ��� ���̾�� �浹
};

