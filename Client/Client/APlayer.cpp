#include "pch.h"
#include "APlayer.h"
#include "CameraComponent.h"
#include "ResourceManager.h"
#include "BoxCollider.h"
#include "DevelopmentScene.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Flipbook.h"

const Vector2Int APlayer::Offset[]{ {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

APlayer::APlayer()
{
	Flipbooks.resize(
		static_cast<size_t>(EPlayerState::END),
		vector<Flipbook*>(static_cast<size_t>(EMovementDirection::END))
	);

	Flipbooks[static_cast<size_t>(EPlayerState::Idle)][static_cast<size_t>(EMovementDirection::Up)]
		= ResourceManager::Get()->GetFlipbook(L"FB_IdleUp");
	Flipbooks[static_cast<size_t>(EPlayerState::Idle)][static_cast<size_t>(EMovementDirection::Down)]
		= ResourceManager::Get()->GetFlipbook(L"FB_IdleDown");
	Flipbooks[static_cast<size_t>(EPlayerState::Idle)][static_cast<size_t>(EMovementDirection::Left)]
		= ResourceManager::Get()->GetFlipbook(L"FB_IdleLeft");
	Flipbooks[static_cast<size_t>(EPlayerState::Idle)][static_cast<size_t>(EMovementDirection::Right)]
		= ResourceManager::Get()->GetFlipbook(L"FB_IdleRight");

	Flipbooks[static_cast<size_t>(EPlayerState::Move)][static_cast<size_t>(EMovementDirection::Up)]
		= ResourceManager::Get()->GetFlipbook(L"FB_MoveUp");
	Flipbooks[static_cast<size_t>(EPlayerState::Move)][static_cast<size_t>(EMovementDirection::Down)]
		= ResourceManager::Get()->GetFlipbook(L"FB_MoveDown");
	Flipbooks[static_cast<size_t>(EPlayerState::Move)][static_cast<size_t>(EMovementDirection::Left)]
		= ResourceManager::Get()->GetFlipbook(L"FB_MoveLeft");
	Flipbooks[static_cast<size_t>(EPlayerState::Move)][static_cast<size_t>(EMovementDirection::Right)]
		= ResourceManager::Get()->GetFlipbook(L"FB_MoveRight");

	Flipbooks[static_cast<size_t>(EPlayerState::Attack)][static_cast<size_t>(EMovementDirection::Up)]
		= ResourceManager::Get()->GetFlipbook(L"FB_AttackUp");
	Flipbooks[static_cast<size_t>(EPlayerState::Attack)][static_cast<size_t>(EMovementDirection::Down)]
		= ResourceManager::Get()->GetFlipbook(L"FB_AttackDown");
	Flipbooks[static_cast<size_t>(EPlayerState::Attack)][static_cast<size_t>(EMovementDirection::Left)]
		= ResourceManager::Get()->GetFlipbook(L"FB_AttackLeft");
	Flipbooks[static_cast<size_t>(EPlayerState::Attack)][static_cast<size_t>(EMovementDirection::Right)]
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

	TransitionTo(EPlayerState::Move);
	TransitionTo(EPlayerState::Idle);

	MoveTo({ 5, 5 }, true);
}

void APlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
	case EPlayerState::Idle:
		OnIdle(DeltaTime);
		break;
	case EPlayerState::Move:
		OnMove(DeltaTime);
		break;
	case EPlayerState::Attack:
		OnAttack(DeltaTime);
		break;
	}
}

void APlayer::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

void APlayer::OnIdle(float DeltaTime)
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
		if (CurrentState == EPlayerState::Idle)
		{
			UpdateAnimation();
		}

		return;
	}

	Vector2Int Dest = CellPosition + Offset[static_cast<int32>(CurrentDirection)];
	if (CanMoveTo(Dest))
	{
		MoveTo(Dest);
		TransitionTo(EPlayerState::Move);
	}
}

// 여기서는 부드러운 움직임을 보여주기 위한 로직 처리
void APlayer::OnMove(float DeltaTime)
{
	if (HasReachedDest())
	{
		TransitionTo(EPlayerState::Idle);
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

void APlayer::OnAttack(float DeltaTime)
{

}

void APlayer::RotateTo(EMovementDirection NewDirection)
{
	CurrentDirection = NewDirection;
	UpdateAnimation();  // 애니메이션 갱신 호출 반드시 필요
}

void APlayer::UpdateAnimation()
{
	switch (CurrentState)
	{
	case EPlayerState::Idle:
		bKeyPressed
			? SetFlipbook(Flipbooks[static_cast<size_t>(EPlayerState::Move)][static_cast<size_t>(CurrentDirection)])
			: SetFlipbook(Flipbooks[static_cast<size_t>(EPlayerState::Idle)][static_cast<size_t>(CurrentDirection)]);
		break;
	case EPlayerState::Move:
		SetFlipbook(Flipbooks[static_cast<size_t>(EPlayerState::Move)][static_cast<size_t>(CurrentDirection)]);
		break;
	case EPlayerState::Attack:
		SetFlipbook(Flipbooks[static_cast<size_t>(EPlayerState::Attack)][static_cast<size_t>(CurrentDirection)]);
		break;
	}
}

bool APlayer::HasReachedDest() const
{
	Vector2 DestDir = (DestinationPosition - CurrentPosition);
	return DestDir.GetMagnitude() < 5.0f;
}

bool APlayer::CanMoveTo(Vector2Int Dest)
{
	if (DevelopmentScene* Scene = dynamic_cast<DevelopmentScene*>(SceneManager::Get()->GetActiveScene()))
	{
		return Scene->CanMoveTo(Dest);
	}

	return false;
}

// 실제 좌표 이동은 여기서 이루어짐
void APlayer::MoveTo(Vector2Int Dest, bool bTeleport)
{
	CellPosition = Dest;

	if (DevelopmentScene* Scene = dynamic_cast<DevelopmentScene*>(SceneManager::Get()->GetActiveScene()))
	{
		DestinationPosition = Scene->CellToWorld(Dest);

		if (bTeleport)
		{
			CurrentPosition = DestinationPosition;
		}
	}
}

void APlayer::TransitionTo(EPlayerState NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	CurrentState = NewState;
	UpdateAnimation();  // 애니메이션 갱신 호출 반드시 필요
}