#include "pch.h"
#include "APlayer.h"
#include "CameraComponent.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Flipbook.h"

const Vector2Int APlayer::Offset[]{ {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

APlayer::APlayer()
{
	Flipbooks.resize(
		static_cast<size_t>(EObjectStates::END),
		vector<Flipbook*>(static_cast<size_t>(EMovementDirection::END))
	);

	Flipbooks[static_cast<size_t>(EObjectStates::Idle)][static_cast<size_t>(EMovementDirection::Up)]
		= ResourceManager::Get()->GetFlipbook(L"FB_IdleUp");
	Flipbooks[static_cast<size_t>(EObjectStates::Idle)][static_cast<size_t>(EMovementDirection::Down)]
		= ResourceManager::Get()->GetFlipbook(L"FB_IdleDown");
	Flipbooks[static_cast<size_t>(EObjectStates::Idle)][static_cast<size_t>(EMovementDirection::Left)]
		= ResourceManager::Get()->GetFlipbook(L"FB_IdleLeft");
	Flipbooks[static_cast<size_t>(EObjectStates::Idle)][static_cast<size_t>(EMovementDirection::Right)]
		= ResourceManager::Get()->GetFlipbook(L"FB_IdleRight");

	Flipbooks[static_cast<size_t>(EObjectStates::Move)][static_cast<size_t>(EMovementDirection::Up)]
		= ResourceManager::Get()->GetFlipbook(L"FB_MoveUp");
	Flipbooks[static_cast<size_t>(EObjectStates::Move)][static_cast<size_t>(EMovementDirection::Down)]
		= ResourceManager::Get()->GetFlipbook(L"FB_MoveDown");
	Flipbooks[static_cast<size_t>(EObjectStates::Move)][static_cast<size_t>(EMovementDirection::Left)]
		= ResourceManager::Get()->GetFlipbook(L"FB_MoveLeft");
	Flipbooks[static_cast<size_t>(EObjectStates::Move)][static_cast<size_t>(EMovementDirection::Right)]
		= ResourceManager::Get()->GetFlipbook(L"FB_MoveRight");

	Flipbooks[static_cast<size_t>(EObjectStates::Attack)][static_cast<size_t>(EMovementDirection::Up)]
		= ResourceManager::Get()->GetFlipbook(L"FB_AttackUp");
	Flipbooks[static_cast<size_t>(EObjectStates::Attack)][static_cast<size_t>(EMovementDirection::Down)]
		= ResourceManager::Get()->GetFlipbook(L"FB_AttackDown");
	Flipbooks[static_cast<size_t>(EObjectStates::Attack)][static_cast<size_t>(EMovementDirection::Left)]
		= ResourceManager::Get()->GetFlipbook(L"FB_AttackLeft");
	Flipbooks[static_cast<size_t>(EObjectStates::Attack)][static_cast<size_t>(EMovementDirection::Right)]
		= ResourceManager::Get()->GetFlipbook(L"FB_AttackRight");

	AddComponent(new CameraComponent());
}

APlayer::~APlayer()
{
	for (vector<Flipbook*> FlipbooksByState : Flipbooks)
	{
		for (Flipbook* FlipbookByDirection : FlipbooksByState)
		{
			SAFE_DELETE(FlipbookByDirection);
		}

		FlipbooksByState.clear();
	}

	Flipbooks.clear();
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();

	MoveTo({ 5, 5 }, true);
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
		if (CurrentState == EObjectStates::Idle)
		{
			UpdateAnimation();
		}

		return;
	}

	Vector2Int Dest = CellPosition + Offset[static_cast<int32>(CurrentDirection)];
	if (CanMoveTo(Dest))
	{
		MoveTo(Dest);
		TransitionTo(EObjectStates::Move);
	}
}

// 여기서는 부드러운 움직임을 보여주기 위한 로직 처리
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

}

void APlayer::UpdateAnimation()
{
	switch (CurrentState)
	{
	case EObjectStates::Idle:
		bKeyPressed
			? SetFlipbook(Flipbooks[static_cast<size_t>(EObjectStates::Move)][static_cast<size_t>(CurrentDirection)])
			: SetFlipbook(Flipbooks[static_cast<size_t>(EObjectStates::Idle)][static_cast<size_t>(CurrentDirection)]);
		break;
	case EObjectStates::Move:
		SetFlipbook(Flipbooks[static_cast<size_t>(EObjectStates::Move)][static_cast<size_t>(CurrentDirection)]);
		break;
	case EObjectStates::Attack:
		SetFlipbook(Flipbooks[static_cast<size_t>(EObjectStates::Attack)][static_cast<size_t>(CurrentDirection)]);
		break;
	}
}