#include "pch.h"
#include "UObject.h"
#include "DevelopmentScene.h"
#include "SceneManager.h"

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
