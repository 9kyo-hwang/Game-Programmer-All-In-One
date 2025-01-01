#include "pch.h"
#include "AMonster.h"
#include "GameZone.h"
#include "APlayer.h"

AMonster::AMonster()
{
	// DataSheet���� �о�� ����...?
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
	// Client���� Scene �˻��ϵ�, Server������ Zone �˻�
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
	if (Distance == 1)  // �� �� ĭ �������� ����
	{
		// TODO: Attack
		WaitUntil = GetTickCount64() + 1000;  // ���� Tick + 1��
		RotateTo(FindLookAtDirection(Player->GetCellPosition()));
		TransitionTo(EObjectStates::Attack, true);
		return;
	}

	vector<Vector2Int> Path;
	if (EnteredZone->FindPath(GetCellPosition(), Player->GetCellPosition(), OUT Path))
	{
		if (Path.size() > 1)  // 0���� �� ��ġ�̹Ƿ� 1������ Ÿ�� ��ġ
		{
			Vector2Int Dest = Path[1];
			if (EnteredZone->CanMoveTo(Dest))
			{
				WaitUntil = GetTickCount64() + 1000;  // ���� Tick + 1��
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
	// ������ �̵��ϴ� ó���� ���� ���, �̵��ϴ� �ð���ŭ ��ٸ����� ����
	// ���� WaitUntil�� (����ð� + 1��)�̱� ������, 1�� �Ŀ� ���°� ���ϴ� ��
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
