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
	return false;
}

bool Collider::OnCollision(BoxCollider* Box1, BoxCollider* Box2)
{
	Vector2 Position1 = Box1->GetOwner()->GetPosition();
	Vector2 Size1 = Box1->Size;

	Vector2 Position2 = Box2->GetOwner()->GetPosition();
	Vector2 Size2 = Box2->Size;

	float MinX1 = Position1.X - Size1.X / 2;
	float MaxX1 = Position1.X + Size1.X / 2;
	float MinY1 = Position1.Y - Size1.Y / 2;
	float MaxY1 = Position1.Y + Size1.Y / 2;

	float MinX2 = Position2.X - Size2.X / 2;
	float MaxX2 = Position2.X + Size2.X / 2;
	float MinY2 = Position2.Y - Size2.Y / 2;
	float MaxY2 = Position2.Y + Size2.Y / 2;

	if (MaxX2 < MinX1
		|| MaxX1 < MinX2
		|| MaxY1 < MinY2
		|| MaxY2 < MinY1)
	{
		return false;
	}

	return true;
}

bool Collider::OnCollision(SphereCollider* Sphere, BoxCollider* Box)
{
	// TODO.
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
