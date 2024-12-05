#include "pch.h"
#include "Player.h"
#include "Bullet.h"

#include "InputManager.h"
#include "ObjectManager.h"
#include "TimerManager.h"

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
	Position.x = 400;
	Position.y = 500;
}

void Player::Update()
{
	float DeltaTime = TimerManager::Get()->GetDeltaTime();
	// d = t * s

	// 현재 이 방식으로는 대각선 이동 시 속도가 더 빨라짐
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
		// TODO: 미사일 발사

		// 새로 생성한 Missile을 화면에 어떻게 그릴 것인가?
		Bullet* NewBullet = ObjectManager::Get()->NewObject<Bullet>();
		NewBullet->SetPosition(Position);  // 플레이어 위치에 생성, 위로 발사
		ObjectManager::Get()->Add(NewBullet);
	}
}

void Player::Render(HDC InDC)
{
	Utils::DrawCircle(InDC, Position, 50);
}
