#include "pch.h"
#include "APlayer.h"

#include "InputManager.h"

APlayer::APlayer()
{
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();
}

void APlayer::Tick()
{
	Super::Tick();

	// TODO: 필요한 기능 구현
	if (InputManager::Get()->GetButton(EKeyCode::W))
	{
		// 위로 이동...
	}
}

void APlayer::Render(HDC InDC)
{
	Super::Render(InDC);
}
