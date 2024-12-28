#include "pch.h"
#include "ALocalPlayer.h"

#include "CameraComponent.h"
#include "InputManager.h"

ALocalPlayer::ALocalPlayer()
{
	// 실제로 조종하는 내 플레이어에 카메라를 붙여야 함
	AddComponent(new CameraComponent());
}

ALocalPlayer::~ALocalPlayer()
{
}

void ALocalPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void ALocalPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALocalPlayer::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

void ALocalPlayer::Input()
{
	bKeyPressed = true;
	if (InputManager::Get()->GetButton(EKeyCode::W))
	{
		RotateTo(EMovementDirection::Up);
	}
	else if (InputManager::Get()->GetButton(EKeyCode::S))
	{
		RotateTo(EMovementDirection::Down);
	}
	else if (InputManager::Get()->GetButton(EKeyCode::A))
	{
		RotateTo(EMovementDirection::Left);
	}
	else if (InputManager::Get()->GetButton(EKeyCode::D))
	{
		RotateTo(EMovementDirection::Right);
	}
	else
	{
		bKeyPressed = false;
	}

	// 무기 바꾸는 것은 일단은 냅둠
	if (InputManager::Get()->GetButtonDown(EKeyCode::Alpha1))
	{
		ChangeWeapon(EWeapons::Sword);
	}
	else if (InputManager::Get()->GetButtonDown(EKeyCode::Alpha2))
	{
		ChangeWeapon(EWeapons::Bow);
	}
	else if (InputManager::Get()->GetButtonDown(EKeyCode::Alpha3))
	{
		ChangeWeapon(EWeapons::Staff);
	}

	if (InputManager::Get()->GetButton(EKeyCode::Space))
	{
		TransitionTo(EObjectStates::Attack);
	}
}

bool ALocalPlayer::Move()
{
	if (!bKeyPressed)
	{
		return false;
	}

	// 현재 방식은 클라이언트가 먼저 이동하고 서버에 후통보
	Vector2Int FrontCell = GetFrontCell();
	if (CanMoveTo(FrontCell))
	{
		MoveTo(FrontCell);
		TransitionTo(EObjectStates::Move);
	}
	return true;
}

void ALocalPlayer::OnTickIdle(float DeltaTime)
{
	Input();  // 입력과 방향 전환만 수행
	Move();  // 실제로 이동
}

void ALocalPlayer::OnTickMove(float DeltaTime)
{
	Super::OnTickMove(DeltaTime);
}

void ALocalPlayer::OnTickAttack(float DeltaTime)
{
	Super::OnTickAttack(DeltaTime);
}
