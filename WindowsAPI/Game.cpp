#include "pch.h"
#include "Game.h"
#include "TimerManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"

Game::Game()
{
}

Game::~Game()
{
	SceneManager::Get()->Clear();
	ResourceManager::Get()->Clear();

	_CrtDumpMemoryLeaks();  // ��� �������� �־��...
}

void Game::Initialize(HWND InWindowHandle)
{
	WindowHandle = InWindowHandle;
	DeviceContextHandle = ::GetDC(InWindowHandle);

	::GetClientRect(WindowHandle, &Rectangle);
	BackDC = ::CreateCompatibleDC(DeviceContextHandle);  // ���� DC�� ȣȯ�Ǵ� �� DC ����
	BackBitmap = ::CreateCompatibleBitmap(DeviceContextHandle, Rectangle.right, Rectangle.bottom);  // ���� DC�� ȣȯ�Ǵ� Bmp ����
	HBITMAP PrevBitmap = (HBITMAP)::SelectObject(BackDC, BackBitmap);  // ���ο� DC�� Bmp�� ����ϵ��� ����, ������� �ʰ� �� ���� Bitmap ��ȯ
	::DeleteObject(PrevBitmap);  // ��ȯ���� ���� Bitmap ����

	TimerManager::Get()->Initialize();
	InputManager::Get()->Initialize(InWindowHandle);
	SceneManager::Get()->Initialize();
	ResourceManager::Get()->Initialize(InWindowHandle, fs::path(L"E:\\Github\\Game-Programmer-All-In-One\\Resources"));
	SoundManager::Get()->Initialize(InWindowHandle);
	// ���ʷ� ȣ��Ǵ� ��
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
	::BitBlt(DeviceContextHandle, 0, 0, Rectangle.right, Rectangle.bottom, BackDC, 0, 0, SRCCOPY);  // ȭ�� ����� ����Ʈ ���۰� ����ϹǷ� �� ���� ������ ��� ����
	::PatBlt(BackDC, 0, 0, Rectangle.right, Rectangle.bottom, WHITENESS);  // ������ �� �� ���۴� �ʱ�ȭ
}
