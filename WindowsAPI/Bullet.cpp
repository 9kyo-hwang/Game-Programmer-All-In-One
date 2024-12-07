#include "pch.h"
#include "Bullet.h"

#include "ObjectManager.h"
#include "TimerManager.h"

Bullet::Bullet()
	: Super(EObjectType::Projectile)
{
}

Bullet::~Bullet()
{
}

void Bullet::Initialize()
{
	// TODO: Data
	Stat.Hp = 1;
	Stat.MaxHp = 1;
	Stat.Speed = 600;
}

void Bullet::Update()
{
	float DeltaTime = TimerManager::Get()->GetDeltaTime();

	// TODO: °¢µµ Àû¿ë
	Position.x += Stat.Speed * DeltaTime * ::cos(Angle);
	Position.y -= Stat.Speed * DeltaTime * ::sin(Angle);

	// TODO: Collision
	const vector<Object*> Objects = ObjectManager::Get()->GetObjects();
	for (Object* Element : Objects)
	{
		if (Element == this)
		{
			continue;
		}

		if (Element->GetType() != EObjectType::Monster)
		{
			continue;
		}

		Vector2 MyPosition = GetPosition();
		Vector2 TargetPosition = Element->GetPosition();

		const float dx = MyPosition.x - TargetPosition.x;
		const float dy = MyPosition.y - TargetPosition.y;
		float Distance = sqrt(dx * dx + dy * dy);

		if (Distance < 25)
		{
			// ±¦ÂúÀ»±î?
			ObjectManager::Get()->Remove(Element);
			ObjectManager::Get()->Remove(this);
			return;
		}
	}

	// TODO: Remove
	if (Position.y < -200)
	{
		ObjectManager::Get()->Remove(this);
		return;
	}
}

void Bullet::Render(HDC InDC)
{
	Utils::DrawCircle(InDC, Position, 25);
}
