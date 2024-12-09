#include "pch.h"
#include "Player.h"
#include "Bullet.h"

#include "InputManager.h"
#include "MeshLine.h"
#include "ObjectManager.h"
#include "TimerManager.h"
#include "ResourceManager.h"  // �ӽ÷� �ϵ� �ڵ�

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

	if (!bIsLocalPlayer)  // �� �÷��̾ �ƴϸ� �������� ����
	{
		return;
	}

	// ���� �� ������δ� �밢�� �̵� �� �ӵ��� �� ������
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
	if (InputManager::Get()->GetButton(EKeyCode::W))
	{
		//Position.Y -= Stat.Speed * DeltaTime;
	}
	if (InputManager::Get()->GetButton(EKeyCode::S))
	{
		//Position.Y += Stat.Speed * DeltaTime;
	}

	if (InputManager::Get()->GetButton(EKeyCode::Q))
	{
	}
	if (InputManager::Get()->GetButton(EKeyCode::E))
	{
	}

	if (InputManager::Get()->GetButtonDown(EKeyCode::Space))
	{
		// TODO: �̻��� �߻�
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

	HPEN Pen = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN OldPen = (HPEN)SelectObject(InDC, Pen);

	//Utils::DrawLine(InDC, Position, /*End of Cannon*/GetFirePosition());

	::SelectObject(InDC, OldPen);
	::DeleteObject(Pen);
}