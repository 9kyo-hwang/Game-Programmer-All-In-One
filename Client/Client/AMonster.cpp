#include "pch.h"
#include "AMonster.h"
#include "ResourceManager.h"

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
