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

public:  // 임시로 public
	// Overlap Begin ~ End 사이에, 겹친 상대방을 기록하기 위한 저장소
	unordered_set<Collider*> CollidedActorsSet;

	// 나는 누구인가? 누구랑 충돌할 것인가?
	ECollideLayer Layer = ECollideLayer::Object;
	// 충돌 관계를 정의하기 위한 비트 플래그
	uint32 CollisionFlag = 0;  // 최초는 모든 레이어와 충돌
};

