#include "pch.h"
#include "CollisionManager.h"

#include "AActor.h"
#include "Collider.h"

void CollisionManager::Initialize()
{
}

void CollisionManager::Update()
{
	// 매 프레임마다 물체 간 Hit/Overlap 판정
	for (Collider*& Src : Colliders)
	{
		for (Collider*& Dest : Colliders)
		{
			if (Src == Dest)
			{
				continue;
			}

			if (Src->IsCollision(Dest))
			{
				// 여기는 엔진단 코드
				if (!Src->CollidedActorsSet.contains(Dest))
				{
					Src->GetOwner()->OnComponentBeginOverlap(Src, Dest);
					Src->CollidedActorsSet.insert(Dest);
					Dest->GetOwner()->OnComponentBeginOverlap(Dest, Src);
					Dest->CollidedActorsSet.insert(Src);
				}
			}
			else
			{
				if (Src->CollidedActorsSet.contains(Dest))
				{
					Src->GetOwner()->OnComponentEndOverlap(Src, Dest);
					Src->CollidedActorsSet.erase(Dest);
					Dest->GetOwner()->OnComponentEndOverlap(Dest, Src);
					Dest->CollidedActorsSet.erase(Src);
				}
			}
		}
	}
}

void CollisionManager::AddCollider(Collider* NewCollider)
{
	if (NewCollider)
	{
		Colliders.push_back(NewCollider);
	}
}

void CollisionManager::RemoveCollider(Collider* TargetCollider)
{
	if (TargetCollider)
	{
		std::erase(Colliders, TargetCollider);
	}
}
