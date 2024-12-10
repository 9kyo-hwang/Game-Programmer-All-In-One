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

	// Gravity. +y�� �Ʒ��� �������� ��
	Speed.Y += 980.665f * DeltaTime;  // v = at

	// Wind.
	float Wind = UIManager::Get()->GetWindPercent();
	Speed.X += 5 * DeltaTime * Wind;

	// Move. ���� �����̹Ƿ� �������� �̵�
	Position += Speed * DeltaTime;

	// Collision.
	for (Object* Element : ObjectManager::Get()->GetObjects())
	{
		if (Element->GetObjectType() != EObjectType::Player)
		{
			continue;
		}

		// Owner�� �����ؾ� ��
		if (Element == Owner)
		{
			continue;
		}

		// EnemyType���� ���� ����� �����ϴ� �͵� ���� ���
		Vector Dir = Position - Element->GetPosition();
		// ��ź�� ������ �´�Ұų� ���ƴٸ�
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
		// TODO: GameplayScene�� �����ϴ� FlipTurn �Լ��� ȣ�� -> SceneManager
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
