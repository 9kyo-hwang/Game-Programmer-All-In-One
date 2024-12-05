#include "pch.h"
#include "Game.h"
#include "TimerManager.h"
#include "InputManager.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Initialize(HWND NewHandleWindow)
{
	HandleWindow = NewHandleWindow;
	HandleDeviceContext = ::GetDC(HandleWindow);

	TimerManager::Get()->Initialize();
	InputManager::Get()->Initialize(HandleWindow);
}

void Game::Update()
{
	TimerManager::Get()->Update();
	InputManager::Get()->Update();
}

void Game::Render()
{
	uint32 FPS = TimerManager::Get()->GetFPS();
	float DeltaTime = TimerManager::Get()->GetDeltaTime();

	{
		wstring Text = std::format(L"FPS({0}), DeltaTime({1}ms)", FPS, static_cast<int32>(DeltaTime * 1000));
		::TextOut(HandleDeviceContext, 650, 10, Text.c_str(), static_cast<int32>(Text.size()));
	}
	
	::Rectangle(HandleDeviceContext, 200, 200, 400, 400);

	{
		POINT MousePosition = InputManager::Get()->GetMousePosition();
		wstring Text = std::format(L"Mouse Position({0}, {1})", MousePosition.x, MousePosition.y);
		::TextOut(HandleDeviceContext, 20, 10, Text.c_str(), static_cast<int32>(Text.size()));
	}
}
