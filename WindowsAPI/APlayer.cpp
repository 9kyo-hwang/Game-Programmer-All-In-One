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

	// TODO: �ʿ��� ��� ����
	if (InputManager::Get()->GetButton(EKeyCode::W))
	{
		// ���� �̵�...
	}
}

void APlayer::Render(HDC InDC)
{
	Super::Render(InDC);
}
