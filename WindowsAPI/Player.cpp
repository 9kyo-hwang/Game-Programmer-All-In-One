#include "pch.h"
#include "Player.h"
#include "Bullet.h"

#include "InputManager.h"
#include "MeshLine.h"
#include "ObjectManager.h"
#include "TimerManager.h"
#include "ResourceManager.h"  // �ӽ÷� �ϵ� �ڵ�
#include "UIManager.h"

Player::Player()
	: Super(EObjectType::Player)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	// �������� ��� �ٸ� �����ʹ� DB�� ����
	// ��ΰ� �������� ��� �ִ� �����ʹ� ��Ʈ�� ����
	Stat.Hp = 100;
	Stat.MaxHp = 100;
	Stat.Speed = 500;
	Position.X = 400;
	Position.Y = 500;
}

void Player::Update()
{
	float DeltaTime = TimerManager::Get()->GetDeltaTime();
	// d = t * s

	// �� �÷��̾ �ƴϸ� �������� ����
	if (!bIsLocalPlayer)
	{
		return;
	}

	OnUpdateFireAngle();

	if (InputManager::Get()->GetButton(EKeyCode::A))
	{
		Position.X -= Stat.Speed * DeltaTime;
		Direction = EDirection::Left;
	}
	if (InputManager::Get()->GetButton(EKeyCode::D))
	{
		Position.X += Stat.Speed * DeltaTime;
		Direction = EDirection::Right;
	}

	// ���� ���� ����
	if (InputManager::Get()->GetButton(EKeyCode::W))
	{
		FireAngle = ::clamp(FireAngle + 50 * DeltaTime, 0.f, 75.f);
	}
	if (InputManager::Get()->GetButton(EKeyCode::S))
	{
		FireAngle = ::clamp(FireAngle - 50 * DeltaTime, 0.f, 75.f);
	}

	// �Ŀ� ������ �ø���
	if (InputManager::Get()->GetButton(EKeyCode::Space))
	{
		float Power = UIManager::Get()->GetPowerPercent();
		Power = min(100, Power + 100 * DeltaTime);
		UIManager::Get()->SetPowerPercent(Power);
	}

	// �߻�
	if (InputManager::Get()->GetButtonUp(EKeyCode::Space))
	{
		// �� �̻� �������� ���ϵ��� ��� ����
		bIsLocalPlayer = false;

		float Power = UIManager::Get()->GetPowerPercent();
		float Speed = 10 * Power;  // �Ŀ��� ����� �߻� �ӵ�
		float Angle = UIManager::Get()->GetBarrelAngle();

		// TODO
		Bullet* NewBullet = ObjectManager::Get()->NewObject<Bullet>();
		NewBullet->SetPosition(Position);
		NewBullet->SetSpeed({ Speed * ::cos(Angle * PI / 180), -1 * Speed * ::sin(Angle * PI / 180)});
		ObjectManager::Get()->Add(NewBullet);
	}
}

void Player::Render(HDC InDC)
{
	if (Direction == EDirection::Left)
	{
		// ������ �ٶ󺸴� ���� �⺻ �̹���
		if (const MeshLine* Mesh = ResourceManager::Get()->GetMeshLine(GetPlayerName()))
		{
			Mesh->Render(InDC, Position, 0.5f, 0.5f);
		}
	}
	else
	{
		// �¿� ������ ���� X�� ���� ���� ����
		if (const MeshLine* Mesh = ResourceManager::Get()->GetMeshLine(GetPlayerName()))
		{
			Mesh->Render(InDC, Position, -0.5f, 0.5f);
		}
	}

	if (bIsLocalPlayer)
	{
		RECT Rect
		{
			static_cast<LONG>(Position.X - 10),
			static_cast<LONG>(Position.Y - 80),
			static_cast<LONG>(Position.X + 10),
			static_cast<LONG>(Position.Y - 60)
		};

		HBRUSH Brush = ::CreateSolidBrush(RGB(250, 236, 197));
		HBRUSH OldBrush = static_cast<HBRUSH>(::SelectObject(InDC, Brush));
		::Ellipse(InDC, Rect.left, Rect.top, Rect.right, Rect.bottom);

		::SelectObject(InDC, OldBrush);
		::DeleteObject(Brush);
	}
}

// ������ ���� ��ȭ �� �����ϴ� �ڵ带 ���� �Լ��� �и�
void Player::OnUpdateFireAngle()
{
	if (Direction == EDirection::Left)
	{
		UIManager::Get()->SetPlayerAngle(180.f);
		UIManager::Get()->SetBarrelAngle(180.f - FireAngle);
	}
	else
	{
		UIManager::Get()->SetPlayerAngle(0.f);
		UIManager::Get()->SetBarrelAngle(FireAngle);
	}
}
