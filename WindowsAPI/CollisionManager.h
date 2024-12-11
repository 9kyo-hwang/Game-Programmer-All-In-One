#pragma once
#include "Singleton.h"

class Collider;

class CollisionManager : public Singleton<CollisionManager>
{
public:
	void Initialize();
	void Update();

	void AddCollider(Collider* NewCollider);
	void RemoveCollider(Collider* TargetCollider);

private:
	// 레이어 별로 나누는 것이 좋음
	vector<Collider*> Colliders;
};

