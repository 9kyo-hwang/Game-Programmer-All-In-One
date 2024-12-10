#include "pch.h"
#include "Bullet.h"

#include "GameplayScene.h"
#include "ObjectManager.h"
#include "SceneManager.h"
#include "TimerManager.h"
#include "UIManager.h"

Bullet::Bullet()
	: Super(EObjectType::Projectile)
{
}

Bullet::~Bullet()
{
}

void Bullet::Initialize()
{
	Radius = 20.f;
}

void Bullet::Update()
{
	float DeltaTime = TimerManager::Get()->GetDeltaTime();

	// Gravity. +y가 아래로 내려가는 것
	Speed.Y += 980.665f * DeltaTime;  // v = at

	// Wind.
	float Wind = UIManager::Get()->GetWindPercent();
	Speed.X += 5 * DeltaTime * Wind;

	// Move. 방향 벡터이므로 더해지면 이동
	Position += Speed * DeltaTime;

	// Collision.
	for (Object* Element : ObjectManager::Get()->GetObjects())
	{
		if (Element->GetObjectType() != EObjectType::Player)
		{
			continue;
		}

		// Owner를 구분해야 함
		if (Element == Owner)
		{
			continue;
		}

		// EnemyType으로 공격 대상을 구분하는 것도 좋은 방법
		Vector Dir = Position - Element->GetPosition();
		// 포탄과 상대방이 맞닿았거나 겹쳤다면
		if (Dir.GetMagnitude() < Radius + Element->GetRadius())
		{
			if (GameplayScene* Scene = dynamic_cast<GameplayScene*>(SceneManager::Get()->GetActiveScene()))
			{
				Scene->FlipGameTurn();
			}

			ObjectManager::Get()->Remove(this);
			return;
		}
	}

	// Delete
	if (Position.Y > GWinSizeY * 1.5 || Position.Y < -GWinSizeY * 1.5)
	{
		// TODO: GameplayScene에 존재하는 FlipTurn 함수를 호출 -> SceneManager
		if (GameplayScene* Scene = dynamic_cast<GameplayScene*>(SceneManager::Get()->GetActiveScene()))
		{
			Scene->FlipGameTurn();
		}

		ObjectManager::Get()->Remove(this);
		return;
	}
}

void Bullet::Render(HDC InDC)
{
	Utils::DrawCircle(InDC, Position, static_cast<int32>(Radius));
}
