#include "pch.h"
#include "Utils.h"

void Utils::DrawTextW(HDC DC, Vector Position, const wstring& Text)
{
	::TextOut(DC, Position.X, Position.Y, Text.c_str(), static_cast<int32>(Text.size()));
}

void Utils::DrawRectangle(HDC DC, Vector Position, int32 Width, int32 Height)
{
	::Rectangle(DC, Position.X - Width / 2, Position.Y - Height / 2, Position.X + Width / 2, Position.Y + Height / 2);
}

void Utils::DrawCircle(HDC DC, Vector Position, int32 Radius)
{
	::Ellipse(DC, Position.X - Radius, Position.Y - Radius, Position.X + Radius, Position.Y + Radius);
}

void Utils::DrawLine(HDC DC, Vector From, Vector To)
{
	::MoveToEx(DC, From.X, From.Y, nullptr);
	::LineTo(DC, To.X, To.Y);
}
