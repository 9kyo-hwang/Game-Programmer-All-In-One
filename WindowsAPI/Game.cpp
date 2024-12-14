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

	_CrtDumpMemoryLeaks();  // 사실 마지막에 넣어야...
}

void Game::Initialize(HWND InWindowHandle)
{
	WindowHandle = InWindowHandle;
	DeviceContextHandle = ::GetDC(InWindowHandle);

	::GetClientRect(WindowHandle, &Rectangle);
	BackDC = ::CreateCompatibleDC(DeviceContextHandle);  // 기존 DC와 호환되는 새 DC 생성
	BackBitmap = ::CreateCompatibleBitmap(DeviceContextHandle, Rectangle.right, Rectangle.bottom);  // 기존 DC에 호환되는 Bmp 생성
	HBITMAP PrevBitmap = (HBITMAP)::SelectObject(BackDC, BackBitmap);  // 새로운 DC와 Bmp를 사용하도록 설정, 사용하지 않게 된 구형 Bitmap 반환
	::DeleteObject(PrevBitmap);  // 반환받은 구형 Bitmap 제거

	TimerManager::Get()->Initialize();
	InputManager::Get()->Initialize(InWindowHandle);
	SceneManager::Get()->Initialize();
	ResourceManager::Get()->Initialize(InWindowHandle, fs::path(L"E:\\Github\\Game-Programmer-All-In-One\\Resources"));
	SoundManager::Get()->Initialize(InWindowHandle);
	// 최초로 호출되는 씬
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

	SceneManager::Get()->Render(BackDC);  // 물체를 그리는 것은 백 버퍼에 수행
	::BitBlt(DeviceContextHandle, 0, 0, Rectangle.right, Rectangle.bottom, BackDC, 0, 0, SRCCOPY);  // 화면 출력은 프론트 버퍼가 담당하므로 백 버퍼 내용을 고속 복사
	::PatBlt(BackDC, 0, 0, Rectangle.right, Rectangle.bottom, WHITENESS);  // 복사한 뒤 백 버퍼는 초기화
}
