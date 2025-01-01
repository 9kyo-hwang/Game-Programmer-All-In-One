#include "pch.h"
#include "AMonster.h"
#include "GameZone.h"
#include "APlayer.h"

AMonster::AMonster()
{
	// DataSheet에서 읽어야 겠지...?
	Info.set_name("MonsterName");
	Info.set_hp(50);
	Info.set_maxhp(50);
	Info.set_attack(5);
	Info.set_defence(0);
}

AMonster::~AMonster()
{
	
}

void AMonster::Initialize()
{
}

void AMonster::Update()
{
	//Super::Update();

	switch (Info.state())
	{
	case Protocol::Idle:
		OnUpdateIdle();
		break;
	case Protocol::Move:
		OnUpdateMove();
		break;
	case Protocol::Attack:
		OnUpdateAttack();
		break;
	}
}

void AMonster::OnUpdateIdle()
{
	// Client에서 Scene 검사하듯, Server에서는 Zone 검사
	if (EnteredZone == nullptr)
	{
		return;
	}

	if (Target.lock() == nullptr)
	{
		Target = EnteredZone->FindNearestPlayerFrom(GetCellPosition());
	}

	TSharedPtr<APlayer> Player = Target.lock();
	if (Player == nullptr)
	{
		return;
	}

	Vector2Int Direction = Player->GetCellPosition() - GetCellPosition();
	int32 Distance = abs(Direction.X) + abs(Direction.Y);
	if (Distance == 1)  // 딱 한 칸 남았으면 공격
	{
		// TODO: Attack
		WaitUntil = GetTickCount64() + 1000;  // 현재 Tick + 1초
		RotateTo(FindLookAtDirection(Player->GetCellPosition()));
		TransitionTo(EObjectStates::Attack, true);
		return;
	}

	vector<Vector2Int> Path;
	if (EnteredZone->FindPath(GetCellPosition(), Player->GetCellPosition(), OUT Path))
	{
		if (Path.size() > 1)  // 0번은 내 위치이므로 1번부터 타겟 위치
		{
			Vector2Int Dest = Path[1];
			if (EnteredZone->CanMoveTo(Dest))
			{
				WaitUntil = GetTickCount64() + 1000;  // 현재 Tick + 1초
				RotateTo(FindLookAtDirection(Dest));
				MoveTo(Dest);
				TransitionTo(EObjectStates::Move, true);
			}
		}
		else
		{
			MoveTo(Path[0]);
		}
	}
}

void AMonster::OnUpdateMove()
{
	// 스르륵 이동하는 처리가 없는 대신, 이동하는 시간만큼 기다리도록 설정
	// 현재 WaitUntil이 (현재시간 + 1초)이기 때문에, 1초 후에 상태가 변하는 것
	int64 Now = GetTickCount64();
	if (Now < WaitUntil)
	{
		return;
	}

	TransitionTo(Protocol::Idle);
}

void AMonster::OnUpdateAttack()
{
}
