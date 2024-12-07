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

	// 현재 이 방식으로는 대각선 이동 시 속도가 더 빨라짐
	if (InputManager::Get()->GetButton(EKeyCode::A))
	{
		Position.X -= Stat.Speed * DeltaTime;
	}
	if (InputManager::Get()->GetButton(EKeyCode::D))
	{
		Position.X += Stat.Speed * DeltaTime;
	}
	if (InputManager::Get()->GetButton(EKeyCode::W))
	{
		Position.Y -= Stat.Speed * DeltaTime;
	}
	if (InputManager::Get()->GetButton(EKeyCode::S))
	{
		Position.Y += Stat.Speed * DeltaTime;
	}

	if (InputManager::Get()->GetButton(EKeyCode::Q))
	{
		CannonAngle += DeltaTime * 10;
	}
	if (InputManager::Get()->GetButton(EKeyCode::E))
	{
		CannonAngle -= DeltaTime * 10;
	}

	if (InputManager::Get()->GetButtonDown(EKeyCode::Space))
	{
		// TODO: 미사일 발사

		// 새로 생성한 Missile을 화면에 어떻게 그릴 것인가?
		Bullet* NewBullet = ObjectManager::Get()->NewObject<Bullet>();
		NewBullet->SetPosition(GetFirePosition());  // 포신 끝 격발 위치에 생성
		NewBullet->SetAngle(CannonAngle);  // 포신 각도를 넘겨줘서 해당 각도로 발사
		ObjectManager::Get()->Add(NewBullet);
	}
}

void Player::Render(HDC InDC)
{
	if (const MeshLine* Mesh = ResourceManager::Get()->GetMeshLine(L"Player"))
	{
		Mesh->Render(InDC, Position);
	}

	HPEN Pen = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN OldPen = (HPEN)SelectObject(InDC, Pen);

	Utils::DrawLine(InDC, Position, /*End of Cannon*/GetFirePosition());

	::SelectObject(InDC, OldPen);
	::DeleteObject(Pen);
}

Vector Player::GetFirePosition()
{
	Vector FirePosition = Position;
	FirePosition.X += CannonLength * ::cos(CannonAngle);
	FirePosition.Y -= CannonLength * ::sin(CannonAngle);  // y좌표는 위로 갈 수록 -

	return FirePosition;
}
