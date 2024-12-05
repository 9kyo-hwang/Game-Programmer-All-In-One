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
	Position.x = 400;
	Position.y = 500;
}

void Player::Update()
{
	float DeltaTime = TimerManager::Get()->GetDeltaTime();
	// d = t * s

	// ���� �� ������δ� �밢�� �̵� �� �ӵ��� �� ������
	if (InputManager::Get()->GetButton(EKeyCode::A))
	{
		Position.x -= Stat.Speed * DeltaTime;
	}
	if (InputManager::Get()->GetButton(EKeyCode::D))
	{
		Position.x += Stat.Speed * DeltaTime;
	}
	if (InputManager::Get()->GetButton(EKeyCode::W))
	{
		Position.y -= Stat.Speed * DeltaTime;
	}
	if (InputManager::Get()->GetButton(EKeyCode::S))
	{
		Position.y += Stat.Speed * DeltaTime;
	}

	if (InputManager::Get()->GetButtonDown(EKeyCode::Space))
	{
		// TODO: �̻��� �߻�

		// ���� ������ Missile�� ȭ�鿡 ��� �׸� ���ΰ�?
		Bullet* NewBullet = ObjectManager::Get()->NewObject<Bullet>();
		NewBullet->SetPosition(Position);  // �÷��̾� ��ġ�� ����, ���� �߻�
		ObjectManager::Get()->Add(NewBullet);
	}
}

void Player::Render(HDC InDC)
{
	if (const MeshLine* Mesh = ResourceManager::Get()->GetMeshLine(L"Player"))
	{
		Mesh->Render(InDC, Position);
	}
}
