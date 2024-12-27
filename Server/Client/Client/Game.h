#pragma once

class Game
{
public:
	Game();
	~Game();

public:
	void Initialize(HWND GameWindowHandle);
	void Update();
	void Render();

private:
	HWND WindowHandle{};
	HDC DeviceContextHandle{};

	// for Back Buffering
private:
	RECT Rectangle{};
	HDC BackDC = nullptr;
	HBITMAP BackBitmap = nullptr;
};

