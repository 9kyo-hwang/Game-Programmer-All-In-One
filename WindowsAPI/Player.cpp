#include "pch.h"
#include "Player.h"
#include "Bullet.h"

#include "InputManager.h"
#include "MeshLine.h"
#include "ObjectManager.h"
#include "TimerManager.h"
#include "ResourceManager.h"  // 임시로 하드 코딩
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

	// 내 플레이어가 아니면 수행하지 않음
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

	// 포신 각도 조절
	if (InputManager::Get()->GetButton(EKeyCode::W))
	{
		FireAngle = ::clamp(FireAngle + 50 * DeltaTime, 0.f, 75.f);
	}
	if (InputManager::Get()->GetButton(EKeyCode::S))
	{
		FireAngle = ::clamp(FireAngle - 50 * DeltaTime, 0.f, 75.f);
	}

	// 파워 게이지 늘리기
	if (InputManager::Get()->GetButton(EKeyCode::Space))
	{
		float Power = UIManager::Get()->GetPowerPercent();
		Power = min(100, Power + 100 * DeltaTime);
		UIManager::Get()->SetPowerPercent(Power);
	}

	// 발사
	if (InputManager::Get()->GetButtonUp(EKeyCode::Space))
	{
		// 더 이상 움직이지 못하도록 즉시 해제
		bIsLocalPlayer = false;

		float Power = UIManager::Get()->GetPowerPercent();
		float Speed = 10 * Power;  // 파워에 비례한 발사 속도
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

// 각도나 방향 변화 시 동작하는 코드를 별도 함수로 분리
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
