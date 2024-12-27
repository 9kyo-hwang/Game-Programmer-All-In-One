#include "pch.h"
#include "AProjectile.h"

AProjectile::AProjectile()
{
}

AProjectile::~AProjectile()
{
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

void AProjectile::OnTickIdle(float DeltaTime)
{
	Super::OnTickIdle(DeltaTime);
}

void AProjectile::OnTickMove(float DeltaTime)
{
	Super::OnTickMove(DeltaTime);
}

void AProjectile::OnTickAttack(float DeltaTime)
{
	Super::OnTickAttack(DeltaTime);
}

void AProjectile::UpdateAnimation()
{
	Super::UpdateAnimation();
}
