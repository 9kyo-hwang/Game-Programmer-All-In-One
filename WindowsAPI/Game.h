#pragma once

class Game
{
public:
	Game();
	~Game();

public:
	void Initialize(HWND NewWindow);
	void Update();
	void Render();

private:
	HWND Window{};
	HDC DC{};

	// for Back Buffering
private:
	RECT Rectangle;
	HDC BackDC;
	HBITMAP BackBitmap;
};

