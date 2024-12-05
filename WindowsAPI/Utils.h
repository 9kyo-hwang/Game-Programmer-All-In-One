#pragma once

class Utils
{
public:
	static void DrawText(HDC HandleDeviceContext, Vector2 Position, const std::wstring& Text);
	static void DrawRectangle(HDC HandleDeviceContext, Vector2 Position, int32 Width, int32 Height);
	static void DrawCircle(HDC HandleDeviceContext, Vector2 Position, int32 Radius);
	static void DrawLine(HDC HandleDeviceContext, Vector2 From, Vector2 To);
};