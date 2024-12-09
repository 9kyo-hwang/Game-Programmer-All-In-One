#include "pch.h"
#include "Player.h"
#include "Bullet.h"

#include "InputManager.h"
#include "MeshLine.h"
#include "ObjectManager.h"
#include "TimerManager.h"
#include "ResourceManager.h"  // 임시로 하드 코딩

Player::Player()
	: Super(EObjectType::Player)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	// 유저마다 모두 다른 데이터는 DB에 저장
	// 모두가 공용으로 들고 있는 데이터는 시트에 저장
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

	if (!bIsLocalPlayer)  // 내 플레이어가 아니면 수행하지 않음
	{
		return;
	}

	// 현재 이 방식으로는 대각선 이동 시 속도가 더 빨라짐
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
		// TODO: 미사일 발사
	}
}

void Player::Render(HDC InDC)
{
	if (Direction == EDirection::Left)
	{
		// 왼쪽을 바라보는 것이 기본 이미지
		if (const MeshLine* Mesh = ResourceManager::Get()->GetMeshLine(GetPlayerName()))
		{
			Mesh->Render(InDC, Position, 0.5f, 0.5f);
		}
	}
	else
	{
		// 좌우 반전을 위해 X축 비율 음수 적용
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