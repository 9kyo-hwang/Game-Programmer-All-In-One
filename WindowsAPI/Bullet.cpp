#include "pch.h"
#include "Bullet.h"

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

	Position.y -= DeltaTime * Stat.Speed;

	// Collision

	// TODO:
}

void Bullet::Render(HDC InDC)
{
	Utils::DrawCircle(InDC, Position, 25);
}
