#include "pch.h"
#include "AMonster.h"

#include "AEffect.h"
#include "APlayer.h"
#include "DevelopmentScene.h"
#include "ResourceManager.h"
#include "SceneManager.h"

AMonster::AMonster()
{
	MoveFlipbooks[static_cast<int32>(EMovementDirection::Up)] = ResourceManager::Get()->GetFlipbook(L"FB_SnakeUp");
	MoveFlipbooks[static_cast<int32>(EMovementDirection::Down)] = ResourceManager::Get()->GetFlipbook(L"FB_SnakeDown");
	MoveFlipbooks[static_cast<int32>(EMovementDirection::Left)] = ResourceManager::Get()->GetFlipbook(L"FB_SnakeLeft");
	MoveFlipbooks[static_cast<int32>(EMovementDirection::Right)] = ResourceManager::Get()->GetFlipbook(L"FB_SnakeRight");

	// 보통 데이터 테이블에서 로드
	AttributeSet.Hp = 50;
	AttributeSet.MaxHp = 50;
	AttributeSet.Attack = 10;
	AttributeSet.Defence = 0;
}

AMonster::~AMonster()
{
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();

	// 최초 상태가 Idle인데, UpdateAnimation가 한 번은 호출해야 함
	// 그런데 최초 Idle == 현재 Idle이라 애니메이션 업데이트가 안됨
	// None이라는 예외 상태를 두는 것보다 Idle과 다른 상태를 한 번 바꾸고 즉시 되돌려놓는 트릭
	TransitionTo(EObjectStates::Move);
	TransitionTo(EObjectStates::Idle);
}

void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMonster::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

void AMonster::OnTickIdle(float DeltaTime)
{
	DevelopmentScene* Scene = dynamic_cast<DevelopmentScene*>(SceneManager::Get()->GetActiveScene());
	if (!Scene)
	{
		return;
	}

	return;  // 모든 로직은 서버로 이관됨

	if (!Target)
	{
		Target = Scene->FindNearestPlayerFrom(GetCellPosition());
	}

	if (!Target)
	{
		// 근처 플레이어를 탐색했는데도 여전히 못찾았으면 Idle 대기
		return;
	}

	Vector2Int Direction = Target->GetCellPosition() - GetCellPosition();
	int32 Distance = abs(Direction.X) + abs(Direction.Y);
	if (Distance == 1)  // 딱 한 칸 남았으면 공격
	{
		// TODO: Attack
		WaitSecs = 0.5f;
		RotateTo(FindLookAtDirection(Target->GetCellPosition()));
		TransitionTo(EObjectStates::Attack);
		return;
	}

	// Chase
	// Idle 상태에서 매 틱마다 길찾기가 수행됨 -> 바로 앞 좌표만 주기적으로 체크
	vector<Vector2Int> Path;
	if (Scene->FindPath(GetCellPosition(), Target->GetCellPosition(), OUT Path))
	{
		if (Path.size() > 1)  // 0번은 내 위치이므로 1번부터 타겟 위치
		{
			Vector2Int Dest = Path[1];
			if (Scene->CanMoveTo(Dest))
			{
				MoveTo(Dest);
				TransitionTo(EObjectStates::Move);
			}
		}
		else
		{
			MoveTo(Path[0]);
		}
	}
}

void AMonster::OnTickMove(float DeltaTime)
{
	if (HasReachedDest())
	{
		TransitionTo(EObjectStates::Idle);
		CurrentPosition = DestinationPosition;
		return;
	}

	// 몬스터는 입력에 의해 방향이 결정되지 않음
	Vector2 Direction = (DestinationPosition - CurrentPosition);
	if (bool bHorizontal = abs(Direction.X) > abs(Direction.Y))
	{
		RotateTo(Direction.X < 0 ? EMovementDirection::Left : EMovementDirection::Right);
	}
	else
	{
		RotateTo(Direction.Y < 0 ? EMovementDirection::Up : EMovementDirection::Down);
	}

	switch (GetCurrentDirection())
	{
	case EMovementDirection::Up:
		CurrentPosition.Y -= 50 * DeltaTime;
		break;
	case EMovementDirection::Down:
		CurrentPosition.Y += 50 * DeltaTime;
		break;
	case EMovementDirection::Left:
		CurrentPosition.X -= 50 * DeltaTime;
		break;
	case EMovementDirection::Right:
		CurrentPosition.X += 50 * DeltaTime;
		break;
	}
}

void AMonster::OnTickAttack(float DeltaTime)
{
	if (!CurrentFlipbook)
	{
		return;
	}

	// 별도의 공격 애니메이션이 엄서...
	// 단순히 애니메이션 끝난 걸로 판별 불가 -> 일정 시간 이후에 종료되도록 하자

	if (WaitSecs > 0)
	{
		WaitSecs = max(0, WaitSecs - DeltaTime);
		return;
	}

	// WaitSecs이 지났으므로 공격!
	if (DevelopmentScene* Scene = dynamic_cast<DevelopmentScene*>(SceneManager::Get()->GetActiveScene()))
	{
		if (APawn* Pawn = Scene->GetPawnAt(GetFrontCell()))
		{
			Scene->NewObject<AEffect>(GetFrontCell());
			Pawn->TakeDamage(this);
		}
	}

	TransitionTo(EObjectStates::Idle);
}

void AMonster::UpdateAnimation()
{
	SetFlipbook(MoveFlipbooks[static_cast<int32>(GetCurrentDirection())]);
}
