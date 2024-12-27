#include "pch.h"
#include "AArrow.h"

#include "AEffect.h"
#include "APawn.h"
#include "DevelopmentScene.h"
#include "ResourceManager.h"
#include "SceneManager.h"

AArrow::AArrow()
{
	MoveFlipbooks[static_cast<int32>(EMovementDirection::Up)] = ResourceManager::Get()->GetFlipbook(L"FB_ArrowUp");
	MoveFlipbooks[static_cast<int32>(EMovementDirection::Down)] = ResourceManager::Get()->GetFlipbook(L"FB_ArrowDown");
	MoveFlipbooks[static_cast<int32>(EMovementDirection::Left)] = ResourceManager::Get()->GetFlipbook(L"FB_ArrowLeft");
	MoveFlipbooks[static_cast<int32>(EMovementDirection::Right)] = ResourceManager::Get()->GetFlipbook(L"FB_ArrowRight");
}

AArrow::~AArrow()
{
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();
	UpdateAnimation();  // 애니메이션이 매 프레임마다 변하는 것이 아니라 BeginPlay에 넣기
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArrow::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

void AArrow::OnTickIdle(float DeltaTime)
{
	Super::OnTickIdle(DeltaTime);

	if (DevelopmentScene* Scene = dynamic_cast<DevelopmentScene*>(SceneManager::Get()->GetActiveScene()))
	{
		Vector2Int NextPosition = GetFrontCell();
		// TODO : 현재 위치도 같이 체크하자!
		if (CanMoveTo(NextPosition))
		{
			MoveTo(NextPosition);
			TransitionTo(EObjectStates::Move);
		}
		else
		{
			if (APawn* Pawn = Scene->GetPawnAt(NextPosition))
			{
				Scene->NewObject<AEffect>(NextPosition);
				Pawn->TakeDamage();  // 공격자는 Pawn만 받는데... 투사체는? -> Owner를 갖고 오자!
			}

			Scene->DestroyActor(this);
		}
	}
}

void AArrow::OnTickMove(float DeltaTime)
{
	Super::OnTickMove(DeltaTime);

	if (HasReachedDest())
	{
		TransitionTo(EObjectStates::Idle);
		SetCurrentPosition(DestinationPosition);
		return;
	}

	// 화살은 한 번 방향이 정해지면 변경되지 않음
	switch (CurrentDirection)
	{
	case EMovementDirection::Up:
		CurrentPosition.Y -= 600 * DeltaTime;
		break;
	case EMovementDirection::Down:
		CurrentPosition.Y += 600 * DeltaTime;
		break;
	case EMovementDirection::Left:
		CurrentPosition.X -= 600 * DeltaTime;
		break;
	case EMovementDirection::Right:
		CurrentPosition.X += 600 * DeltaTime;
		break;
	}
}

void AArrow::UpdateAnimation()
{
	//Super::UpdateAnimation();

	SetFlipbook(MoveFlipbooks[static_cast<size_t>(CurrentDirection)]);
}
