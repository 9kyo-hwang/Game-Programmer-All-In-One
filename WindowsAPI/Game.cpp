#include "pch.h"
#include "Game.h"
#include "TimerManager.h"
#include "InputManager.h"
#include "SceneManager.h"

Game::Game()
{
}

Game::~Game()
{
	SceneManager::Get()->Clear();
	_CrtDumpMemoryLeaks();  // ��� �������� �־��...
}

void Game::Initialize(HWND NewWindow)
{
	Window = NewWindow;
	DC = ::GetDC(NewWindow);

	::GetClientRect(Window, &Rectangle);
	BackDC = ::CreateCompatibleDC(DC);  // ���� DC�� ȣȯ�Ǵ� �� DC ����
	BackBitmap = ::CreateCompatibleBitmap(DC, Rectangle.right, Rectangle.bottom);  // ���� DC�� ȣȯ�Ǵ� Bmp ����
	HBITMAP PrevBitmap = (HBITMAP)::SelectObject(BackDC, BackBitmap);  // ���ο� DC�� Bmp�� ����ϵ��� ����, ������� �ʰ� �� ���� Bitmap ��ȯ
	::DeleteObject(PrevBitmap);  // ��ȯ���� ���� Bitmap ����

	TimerManager::Get()->Initialize();
	InputManager::Get()->Initialize(NewWindow);
	SceneManager::Get()->Initialize();

	SceneManager::Get()->LoadScene(ESceneType::Development);
}

void Game::Update()
{
	TimerManager::Get()->Update();
	InputManager::Get()->Update();
	SceneManager::Get()->Update();
}

void Game::Render()
{
	uint32 FPS = TimerManager::Get()->GetFPS();
	float DeltaTime = TimerManager::Get()->GetDeltaTime();

	{
		wstring Text = std::format(L"FPS({0}), DeltaTime({1}ms)", FPS, static_cast<int32>(DeltaTime * 1000));
		::TextOut(BackDC, 650, 10, Text.c_str(), static_cast<int32>(Text.size()));
	}

	{
		POINT MousePosition = InputManager::Get()->GetMousePosition();
		wstring Text = std::format(L"Mouse Position({0}, {1})", MousePosition.x, MousePosition.y);
		::TextOut(BackDC, 20, 10, Text.c_str(), static_cast<int32>(Text.size()));
	}

	SceneManager::Get()->Render(BackDC);  // ��ü�� �׸��� ���� �� ���ۿ� ����
	::BitBlt(DC, 0, 0, Rectangle.right, Rectangle.bottom, BackDC, 0, 0, SRCCOPY);  // ȭ�� ����� ����Ʈ ���۰� ����ϹǷ� �� ���� ������ ��� ����
	::PatBlt(BackDC, 0, 0, Rectangle.right, Rectangle.bottom, WHITENESS);  // ������ �� �� ���۴� �ʱ�ȭ
}
