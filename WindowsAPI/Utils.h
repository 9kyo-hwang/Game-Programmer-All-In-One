#pragma once

class Utils
{
public:
	static void DrawText(HDC DC, Vector2 Position, const std::wstring& Text);
	static void DrawRectangle(HDC DC, Vector2 Position, int32 Width, int32 Height);
	static void DrawCircle(HDC DC, Vector2 Position, int32 Radius);
	static void DrawLine(HDC DC, Vector2 From, Vector2 To);
};