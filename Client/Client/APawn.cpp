#include "pch.h"
#include "APawn.h"
#include "SceneManager.h"
#include "Scene.h"

APawn::APawn()
{
}

APawn::~APawn()
{
}

void APawn::BeginPlay()
{
	Super::BeginPlay();
}

void APawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APawn::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

void APawn::TakeDamage(APawn* Instigator)
{
	if (!Instigator)
	{
		return;
	}

	UAttributeSet& InstigatorAttributeSet = Instigator->GetAttributeSet();
	int32 Damage = InstigatorAttributeSet.Attack - AttributeSet.Defence;
	if (Damage <= 0)
	{
		return;
	}

	AttributeSet.Hp = max(0, AttributeSet.Hp - Damage);
	if (AttributeSet.Hp == 0)
	{
		if (Scene* ActiveScene = SceneManager::Get()->GetActiveScene())
		{
			ActiveScene->DestroyActor(this);
		}
	}
}
