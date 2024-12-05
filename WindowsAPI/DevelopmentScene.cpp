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

	// ���� �� ������δ� �밢�� �̵� �� �ӵ��� �� ������
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
	// ���� Object ������ �̰��� �ڵ�
	Utils::DrawCircle(InDC, PlayerPosition, 50);
}
