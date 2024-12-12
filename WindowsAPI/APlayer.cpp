#include "pch.h"
#include "APlayer.h"

#include "CameraComponent.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "TimerManager.h"

APlayer::APlayer()
{
	FB_MoveUp = ResourceManager::Get()->GetFlipbook(L"FB_MoveUp");
	FB_MoveDown = ResourceManager::Get()->GetFlipbook(L"FB_MoveDown");
	FB_MoveLeft = ResourceManager::Get()->GetFlipbook(L"FB_MoveLeft");
	FB_MoveRight = ResourceManager::Get()->GetFlipbook(L"FB_MoveRight");

	AddComponent(new CameraComponent());
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();

	SetFlipbook(FB_MoveRight);
}

void APlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO: 필요한 기능 구현
	if (InputManager::Get()->GetButton(EKeyCode::W))
	{
		Position.Y -= 200 * DeltaTime;
		SetFlipbook(FB_MoveUp);
	}
	else if (InputManager::Get()->GetButton(EKeyCode::S))
	{
		Position.Y += 200 * DeltaTime;
		SetFlipbook(FB_MoveDown);
	}
	else if (InputManager::Get()->GetButton(EKeyCode::A))
	{
		Position.X -= 200 * DeltaTime;
		SetFlipbook(FB_MoveLeft);
	}
	else if (InputManager::Get()->GetButton(EKeyCode::D))
	{
		Position.X += 200 * DeltaTime;
		SetFlipbook(FB_MoveRight);
	}
}

void APlayer::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

void APlayer::OnComponentBeginOverlap(Collider* This, Collider* Other)
{
	Super::OnComponentBeginOverlap(This, Other);
}

void APlayer::OnComponentEndOverlap(Collider* This, Collider* Other)
{
	Super::OnComponentEndOverlap(This, Other);
}
