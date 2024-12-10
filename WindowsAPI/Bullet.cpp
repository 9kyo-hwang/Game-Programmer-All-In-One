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

	Position += Speed * DeltaTime;  // ���� �����̹Ƿ� �������� �̵�
	if (Position.Y > GWinSizeY * 1.5 || Position.Y < -GWinSizeY * 1.5)
	{
		// TODO: GameplayScene�� �����ϴ� FlipTurn �Լ��� ȣ�� -> SceneManager
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
