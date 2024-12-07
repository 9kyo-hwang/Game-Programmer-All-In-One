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

	if (!Target)  // Ÿ�� ã��
	{
		Position.X += Stat.Speed * DeltaTime * ::cos(Angle);
		Position.Y -= Stat.Speed * DeltaTime * ::sin(Angle);
		ElapsedTime += DeltaTime;

		if (ElapsedTime >= 0.2f)  // 0.2�� ��� �� ����ź���� ��ȭ
		{
			FindNearestMonster();
		}
	}
	else
	{
		Vector Direction = Target->GetPosition() - GetPosition();
		Direction.Normalize();  // ���⸸ �ʿ�
		Position += Direction * Stat.Speed * DeltaTime;  // �̵��� ������
	}

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

		float Distance = (Element->GetPosition() - GetPosition()).GetMagnitude();
		if (Distance < 25)
		{
			// ��������?
			ObjectManager::Get()->Remove(Element);
			ObjectManager::Get()->Remove(this);
			return;
		}
	}

	// TODO: Remove
	if (Position.Y < -200)
	{
		ObjectManager::Get()->Remove(this);
		return;
	}
}

void Bullet::Render(HDC InDC)
{
	Utils::DrawCircle(InDC, Position, 25);
}

bool Bullet::FindNearestMonster()
{
	const vector<Object*> Objects = ObjectManager::Get()->GetObjects();
	for (Object* Element : Objects)
	{
		if (Element->GetType() == EObjectType::Monster)
		{
			Target = Element;
			return true;
		}
	}

	return false;
}
