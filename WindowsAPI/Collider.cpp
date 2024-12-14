#include "pch.h"
#include "Collider.h"
#include "AActor.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

Collider::Collider(EColliderType InType)
	: Type(InType)
{
}

Collider::~Collider()
{
}

void Collider::BeginPlay()
{
}

void Collider::TickComponent()
{
}

void Collider::Render(HDC InDC)
{
}

bool Collider::IsCollision(Collider* Other)
{
	ECollideLayer OtherLayer = Other->GetCollideLayer();
	if (CollisionFlag & 1 << static_cast<uint32>(OtherLayer))
	{
		return true;
	}

	return false;
}

bool Collider::OnCollision(BoxCollider* Box1, BoxCollider* Box2)
{
	// https://blog.naver.com/winterwolfs/10165506488
	RECT Rect1 = Box1->GetRect();
	RECT Rect2 = Box2->GetRect();

	RECT Intersect{};
	return ::IntersectRect(&Intersect, &Rect1, &Rect2);
}

bool Collider::OnCollision(SphereCollider* Sphere, BoxCollider* Box)
{
	return false;
}

bool Collider::OnCollision(SphereCollider* Sphere1, SphereCollider* Sphere2)
{
	Vector2 Position1 = Sphere1->GetOwner()->GetPosition();
	float Radius1 = Sphere1->Radius;

	Vector2 Position2 = Sphere2->GetOwner()->GetPosition();
	float Radius2 = Sphere2->Radius;

	float Distance = (Position1 - Position2).GetMagnitude();

	return Distance <= Radius1 + Radius2;
}

void Collider::AddCollisionFlagLayer(ECollideLayer InLayer)
{
	CollisionFlag |= (1 << static_cast<uint32>(InLayer));
}

void Collider::RemoveCollisionFlagLayer(ECollideLayer InLayer)
{
	CollisionFlag &= ~(1 << static_cast<uint32>(InLayer));
}
