#include "pch.h"
#include "Utils.h"

void Utils::DrawTextW(HDC HandleDeviceContext, Vector2 Position, const wstring& Text)
{
	::TextOut(HandleDeviceContext, Position.x, Position.y, Text.c_str(), static_cast<int32>(Text.size()));
}

void Utils::DrawRectangle(HDC HandleDeviceContext, Vector2 Position, int32 Width, int32 Height)
{
	::Rectangle(HandleDeviceContext, Position.x - Width / 2, Position.y - Height / 2, Position.x + Width / 2, Position.y + Height / 2);
}

void Utils::DrawCircle(HDC HandleDeviceContext, Vector2 Position, int32 Radius)
{
	::Ellipse(HandleDeviceContext, Position.x - Radius, Position.y - Radius, Position.x + Radius, Position.y + Radius);
}

void Utils::DrawLine(HDC HandleDeviceContext, Vector2 From, Vector2 To)
{
	::MoveToEx(HandleDeviceContext, From.x, From.y, nullptr);
	::LineTo(HandleDeviceContext, To.x, To.y);
}
