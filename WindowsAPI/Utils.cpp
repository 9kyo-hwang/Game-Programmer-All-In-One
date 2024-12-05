#include "pch.h"
#include "Utils.h"

void Utils::DrawTextW(HDC DC, Vector2 Position, const wstring& Text)
{
	::TextOut(DC, Position.x, Position.y, Text.c_str(), static_cast<int32>(Text.size()));
}

void Utils::DrawRectangle(HDC DC, Vector2 Position, int32 Width, int32 Height)
{
	::Rectangle(DC, Position.x - Width / 2, Position.y - Height / 2, Position.x + Width / 2, Position.y + Height / 2);
}

void Utils::DrawCircle(HDC DC, Vector2 Position, int32 Radius)
{
	::Ellipse(DC, Position.x - Radius, Position.y - Radius, Position.x + Radius, Position.y + Radius);
}

void Utils::DrawLine(HDC DC, Vector2 From, Vector2 To)
{
	::MoveToEx(DC, From.x, From.y, nullptr);
	::LineTo(DC, To.x, To.y);
}
