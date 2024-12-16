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
	// ���̾� ���� ������ ���� ����
	vector<Collider*> Colliders;
};

