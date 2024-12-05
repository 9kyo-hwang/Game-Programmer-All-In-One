#pragma once

class Game
{
public:
	Game();
	~Game();

public:
	void Initialize(HWND InHandleWindow);
	void Update();
	void Render();

private:
	HWND HandleWindow{};
	HDC HandleDeviceContext{};
};

