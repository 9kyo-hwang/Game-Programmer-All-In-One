#include "pch.h"
#include "AMonster.h"
#include "ResourceManager.h"

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

}

void AMonster::OnTickMove(float DeltaTime)
{
}

void AMonster::OnTickAttack(float DeltaTime)
{
}

void AMonster::UpdateAnimation()
{
	SetFlipbook(MoveFlipbooks[static_cast<int32>(CurrentDirection)]);
}
