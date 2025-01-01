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

	// ���� ������ ���̺��� �ε�
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

	// ���� ���°� Idle�ε�, UpdateAnimation�� �� ���� ȣ���ؾ� ��
	// �׷��� ���� Idle == ���� Idle�̶� �ִϸ��̼� ������Ʈ�� �ȵ�
	// None�̶�� ���� ���¸� �δ� �ͺ��� Idle�� �ٸ� ���¸� �� �� �ٲٰ� ��� �ǵ������� Ʈ��
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

	return;  // ��� ������ ������ �̰���

	if (!Target)
	{
		Target = Scene->FindNearestPlayerFrom(GetCellPosition());
	}

	if (!Target)
	{
		// ��ó �÷��̾ Ž���ߴµ��� ������ ��ã������ Idle ���
		return;
	}

	Vector2Int Direction = Target->GetCellPosition() - GetCellPosition();
	int32 Distance = abs(Direction.X) + abs(Direction.Y);
	if (Distance == 1)  // �� �� ĭ �������� ����
	{
		// TODO: Attack
		WaitSecs = 0.5f;
		RotateTo(FindLookAtDirection(Target->GetCellPosition()));
		TransitionTo(EObjectStates::Attack);
		return;
	}

	// Chase
	// Idle ���¿��� �� ƽ���� ��ã�Ⱑ ����� -> �ٷ� �� ��ǥ�� �ֱ������� üũ
	vector<Vector2Int> Path;
	if (Scene->FindPath(GetCellPosition(), Target->GetCellPosition(), OUT Path))
	{
		if (Path.size() > 1)  // 0���� �� ��ġ�̹Ƿ� 1������ Ÿ�� ��ġ
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

	// ���ʹ� �Է¿� ���� ������ �������� ����
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

	// ������ ���� �ִϸ��̼��� ����...
	// �ܼ��� �ִϸ��̼� ���� �ɷ� �Ǻ� �Ұ� -> ���� �ð� ���Ŀ� ����ǵ��� ����

	if (WaitSecs > 0)
	{
		WaitSecs = max(0, WaitSecs - DeltaTime);
		return;
	}

	// WaitSecs�� �������Ƿ� ����!
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
