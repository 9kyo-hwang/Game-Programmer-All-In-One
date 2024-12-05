#include "pch.h"
#include "DevelopmentScene.h"
#include "InputManager.h"
#include "TimerManager.h"

DevelopmentScene::DevelopmentScene()
{
}

DevelopmentScene::~DevelopmentScene()
{
}

void DevelopmentScene::Initialize()
{
	
}

void DevelopmentScene::Update()
{
	float DeltaTime = TimerManager::Get()->GetDeltaTime();
	// d = t * s

	// 현재 이 방식으로는 대각선 이동 시 속도가 더 빨라짐
	if (InputManager::Get()->GetButton(EKeyCode::A))
	{
		PlayerPosition.x -= Speed * DeltaTime;
	}
	if (InputManager::Get()->GetButton(EKeyCode::D))
	{
		PlayerPosition.x += Speed * DeltaTime;
	}
	if (InputManager::Get()->GetButton(EKeyCode::W))
	{
		PlayerPosition.y -= Speed * DeltaTime;
	}
	if (InputManager::Get()->GetButton(EKeyCode::S))
	{
		PlayerPosition.y += Speed * DeltaTime;
	}
}

void DevelopmentScene::Render(HDC InDC)
{
	// 추후 Object 쪽으로 이관될 코드
	Utils::DrawCircle(InDC, PlayerPosition, 50);
}
