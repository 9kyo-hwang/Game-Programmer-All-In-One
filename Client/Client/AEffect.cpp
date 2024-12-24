#include "pch.h"
#include "AEffect.h"

AEffect::AEffect()
{
}

AEffect::~AEffect()
{
}

void AEffect::BeginPlay()
{
	Super::BeginPlay();
}

void AEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEffect::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

void AEffect::UpdateAnimation()
{
	Super::UpdateAnimation();
}
