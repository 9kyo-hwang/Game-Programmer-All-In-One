#pragma once

class UI
{
public:
	UI();
	virtual ~UI();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC DeviceContextHandle);

	RECT GetRectangle();
	bool IsMouseInRectangle();

public:
	Vector2 Position{ 400, 300 };
	Vector2Int Size{ 150, 150 };
};
