#include "pch.h"
#include "AMonster.h"

AMonster::AMonster()
{
}

AMonster::~AMonster()
{
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();
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
}
