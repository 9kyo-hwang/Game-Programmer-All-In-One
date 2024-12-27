#include "pch.h"
#include "AEffect.h"

#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"

AEffect::AEffect()
{
	SetLayer(ERenderLayer::Effect);
}

AEffect::~AEffect()
{
}

void AEffect::BeginPlay()
{
	Super::BeginPlay();

	UpdateAnimation();
}

void AEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAnimationFinished())
	{
		SceneManager::Get()->GetActiveScene()->DestroyActor(this);
	}
}

void AEffect::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

void AEffect::UpdateAnimation()
{
	SetFlipbook(ResourceManager::Get()->GetFlipbook(L"FB_Hit"));
}
