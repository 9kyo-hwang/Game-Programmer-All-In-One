#include "pch.h"
#include "APlayer.h"
#include "CameraComponent.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Flipbook.h"

const Vector2Int APlayer::Offset[]{ {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

APlayer::APlayer()
{
	IdleFlipbooks[static_cast<int32>(EMovementDirection::Up)] = ResourceManager::Get()->GetFlipbook(L"FB_IdleUp");
	IdleFlipbooks[static_cast<int32>(EMovementDirection::Down)] = ResourceManager::Get()->GetFlipbook(L"FB_IdleDown");
	IdleFlipbooks[static_cast<int32>(EMovementDirection::Left)] = ResourceManager::Get()->GetFlipbook(L"FB_IdleLeft");
	IdleFlipbooks[static_cast<int32>(EMovementDirection::Right)] = ResourceManager::Get()->GetFlipbook(L"FB_IdleRight");

	MoveFlipbooks[static_cast<int32>(EMovementDirection::Up)]= ResourceManager::Get()->GetFlipbook(L"FB_MoveUp");
	MoveFlipbooks[static_cast<int32>(EMovementDirection::Down)]= ResourceManager::Get()->GetFlipbook(L"FB_MoveDown");
	MoveFlipbooks[static_cast<int32>(EMovementDirection::Left)]= ResourceManager::Get()->GetFlipbook(L"FB_MoveLeft");
	MoveFlipbooks[static_cast<int32>(EMovementDirection::Right)]= ResourceManager::Get()->GetFlipbook(L"FB_MoveRight");
	 
	SwordFlipbooks[static_cast<int32>(EMovementDirection::Up)]= ResourceManager::Get()->GetFlipbook(L"FB_AttackUp");
	SwordFlipbooks[static_cast<int32>(EMovementDirection::Down)]= ResourceManager::Get()->GetFlipbook(L"FB_AttackDown");
	SwordFlipbooks[static_cast<int32>(EMovementDirection::Left)]= ResourceManager::Get()->GetFlipbook(L"FB_AttackLeft");
	SwordFlipbooks[static_cast<int32>(EMovementDirection::Right)]= ResourceManager::Get()->GetFlipbook(L"FB_AttackRight");

	BowFlipbooks[static_cast<int32>(EMovementDirection::Up)] = ResourceManager::Get()->GetFlipbook(L"FB_BowUp");
	BowFlipbooks[static_cast<int32>(EMovementDirection::Down)] = ResourceManager::Get()->GetFlipbook(L"FB_BowDown");
	BowFlipbooks[static_cast<int32>(EMovementDirection::Left)] = ResourceManager::Get()->GetFlipbook(L"FB_BowLeft");
	BowFlipbooks[static_cast<int32>(EMovementDirection::Right)] = ResourceManager::Get()->GetFlipbook(L"FB_BowRight");

	StaffFlipbooks[static_cast<int32>(EMovementDirection::Up)] = ResourceManager::Get()->GetFlipbook(L"FB_StaffUp");
	StaffFlipbooks[static_cast<int32>(EMovementDirection::Down)] = ResourceManager::Get()->GetFlipbook(L"FB_StaffDown");
	StaffFlipbooks[static_cast<int32>(EMovementDirection::Left)] = ResourceManager::Get()->GetFlipbook(L"FB_StaffLeft");
	StaffFlipbooks[static_cast<int32>(EMovementDirection::Right)] = ResourceManager::Get()->GetFlipbook(L"FB_StaffRight");

	AddComponent(new CameraComponent());
}

APlayer::~APlayer()
{
	for (Flipbook* Idle : IdleFlipbooks)
	{
		SAFE_DELETE(Idle);
	}

	for (Flipbook* Move : MoveFlipbooks)
	{
		SAFE_DELETE(Move);
	}

	for (Flipbook* Sword : SwordFlipbooks)
	{
		SAFE_DELETE(Sword);
	}

	for (Flipbook* Bow : BowFlipbooks)
	{
		SAFE_DELETE(Bow);
	}

	for (Flipbook* Staff : StaffFlipbooks)
	{
		SAFE_DELETE(Staff);
	}
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();
}

void APlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayer::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

void APlayer::OnTickIdle(float DeltaTime)
{
	bKeyPressed = true;
	if (InputManager::Get()->GetButton(EKeyCode::W))
	{
		RotateTo(EMovementDirection::Up);
		Vector2Int Dest = CellPosition + Offset[static_cast<int32>(CurrentDirection)];
		if (CanMoveTo(Dest))
		{
			MoveTo(Dest);
			TransitionTo(EObjectStates::Move);
		}
	}
	else if (InputManager::Get()->GetButton(EKeyCode::S))
	{
		RotateTo(EMovementDirection::Down);
		Vector2Int Dest = CellPosition + Offset[static_cast<int32>(CurrentDirection)];
		if (CanMoveTo(Dest))
		{
			MoveTo(Dest);
			TransitionTo(EObjectStates::Move);
		}
	}
	else if (InputManager::Get()->GetButton(EKeyCode::A))
	{
		RotateTo(EMovementDirection::Left);
		Vector2Int Dest = CellPosition + Offset[static_cast<int32>(CurrentDirection)];
		if (CanMoveTo(Dest))
		{
			MoveTo(Dest);
			TransitionTo(EObjectStates::Move);
		}
	}
	else if (InputManager::Get()->GetButton(EKeyCode::D))
	{
		RotateTo(EMovementDirection::Right);
		Vector2Int Dest = CellPosition + Offset[static_cast<int32>(CurrentDirection)];
		if (CanMoveTo(Dest))
		{
			MoveTo(Dest);
			TransitionTo(EObjectStates::Move);
		}
	}
	else
	{
		bKeyPressed = false;
		if (CurrentState == EObjectStates::Idle)
		{
			UpdateAnimation();
		}
	}

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

// ���⼭�� �ε巯�� �������� �����ֱ� ���� ���� ó��
void APlayer::OnTickMove(float DeltaTime)
{
	if (HasReachedDest())
	{
		TransitionTo(EObjectStates::Idle);
		CurrentPosition = DestinationPosition;
		return;
	}

	switch (CurrentDirection)
	{
	case EMovementDirection::Up:
		CurrentPosition.Y -= 200 * DeltaTime;
		break;
	case EMovementDirection::Down:
		CurrentPosition.Y += 200 * DeltaTime;
		break;
	case EMovementDirection::Left:
		CurrentPosition.X -= 200 * DeltaTime;
		break;
	case EMovementDirection::Right:
		CurrentPosition.X += 200 * DeltaTime;
		break;
	}
}

void APlayer::OnTickAttack(float DeltaTime)
{
	// �ִϸ��̼� ����� ������ �� Ȯ�� �� State Change
	// ���������� ��� �ð��� ������� ���� �Ǻ��ؾ� �ϳ�, Ŭ���̾�Ʈ������ �̺�Ʈ Ű�� ������� �ص� OK(Ÿ�ֿ̹� �°� ����Ʈ ��� ����)
	if (!CurrentFlipbook)
	{
		return;
	}

	if (HasAnimationFinished())
	{
		// TODO: �ǰ� ����
		TransitionTo(EObjectStates::Idle);
	}
}

void APlayer::UpdateAnimation()
{
	switch (CurrentState)
	{
	case EObjectStates::Idle:
		bKeyPressed
			? ChangeFlipbook(MoveFlipbooks[static_cast<size_t>(CurrentDirection)])
			: ChangeFlipbook(IdleFlipbooks[static_cast<size_t>(CurrentDirection)]);
		break;
	case EObjectStates::Move:
		ChangeFlipbook(MoveFlipbooks[static_cast<size_t>(CurrentDirection)]);
		break;
	case EObjectStates::Attack:
		if (CurrentWeapon == EWeapons::Sword)
		{
			ChangeFlipbook(SwordFlipbooks[static_cast<size_t>(CurrentDirection)]);
		}
		else if (CurrentWeapon == EWeapons::Bow)
		{
			ChangeFlipbook(BowFlipbooks[static_cast<size_t>(CurrentDirection)]);
		}
		else if (CurrentWeapon == EWeapons::Staff)
		{
			ChangeFlipbook(StaffFlipbooks[static_cast<size_t>(CurrentDirection)]);
		}
		break;
	}
}