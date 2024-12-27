#include "pch.h"
#include "UObject.h"
#include "DevelopmentScene.h"
#include "SceneManager.h"

const Vector2Int UObject::Offset[]{ {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

UObject::UObject()
{
}

UObject::~UObject()
{
}

void UObject::BeginPlay()
{
	Super::BeginPlay();

	TransitionTo(EObjectStates::Move);
	TransitionTo(EObjectStates::Idle);
}

void UObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
	case EObjectStates::Idle:
		OnTickIdle(DeltaTime);
		break;
	case EObjectStates::Move:
		OnTickMove(DeltaTime);
		break;
	case EObjectStates::Attack:
		OnTickAttack(DeltaTime);
		break;
	}
}

void UObject::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

void UObject::TransitionTo(EObjectStates NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	CurrentState = NewState;
	UpdateAnimation();
}

void UObject::RotateTo(EMovementDirection NewDirection)
{
	CurrentDirection = NewDirection;
	UpdateAnimation();
}

bool UObject::HasReachedDest() const
{
	Vector2 DestDir = (DestinationPosition - CurrentPosition);
	return DestDir.GetMagnitude() < 5.0f;
}

bool UObject::CanMoveTo(Vector2Int Dest)
{
	if (DevelopmentScene* Scene = dynamic_cast<DevelopmentScene*>(SceneManager::Get()->GetActiveScene()))
	{
		return Scene->CanMoveTo(Dest);
	}

	return false;
}

// 실제 좌표 이동은 여기서 이루어짐
void UObject::MoveTo(Vector2Int Dest, bool bTeleport)
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

Vector2Int UObject::GetFrontCell() const
{
	switch (CurrentDirection)
	{
	case EMovementDirection::Up:
		return CellPosition + Offset[static_cast<int32>(EMovementDirection::Up)];
	case EMovementDirection::Down:
		return CellPosition + Offset[static_cast<int32>(EMovementDirection::Down)];
	case EMovementDirection::Left:
		return CellPosition + Offset[static_cast<int32>(EMovementDirection::Left)];
	case EMovementDirection::Right:
		return CellPosition + Offset[static_cast<int32>(EMovementDirection::Right)];
	default:
		return CellPosition;
	}
}

EMovementDirection UObject::FindLookAtDirection(Vector2Int TargetPosition) const
{
	Vector2Int Direction = TargetPosition - GetCellPosition();
	if (Direction.X < 0)
	{
		return EMovementDirection::Left;
	}
	else if (Direction.X > 0)
	{
		return EMovementDirection::Right;
	}
	else if (Direction.Y < 0)
	{
		return EMovementDirection::Up;
	}
	else if (Direction.Y > 0)
	{
		return EMovementDirection::Down;
	}
}
