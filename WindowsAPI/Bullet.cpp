#include "pch.h"
#include "Bullet.h"

#include "GameplayScene.h"
#include "ObjectManager.h"
#include "SceneManager.h"
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
	Radius = 20.f;
}

void Bullet::Update()
{
	float DeltaTime = TimerManager::Get()->GetDeltaTime();

	// Wind, Gravity...

	Position += Speed * DeltaTime;  // 방향 벡터이므로 더해지면 이동
	if (Position.Y > GWinSizeY * 1.5 || Position.Y < -GWinSizeY * 1.5)
	{
		// TODO: GameplayScene에 존재하는 FlipTurn 함수를 호출 -> SceneManager
		if (GameplayScene* Scene = dynamic_cast<GameplayScene*>(SceneManager::Get()->GetActiveScene())
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
