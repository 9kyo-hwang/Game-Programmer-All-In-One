#include "pch.h"
#include "UI.h"

#include "InputManager.h"

UI::UI()
{
}

UI::~UI()
{
}

void UI::BeginPlay()
{
}

void UI::Tick()
{
}

void UI::Render(HDC DeviceContextHandle)
{
}

RECT UI::GetRectangle()
{
	return
	{
		static_cast<LONG>(Position.X - Size.X / 2),
		static_cast<LONG>(Position.Y - Size.Y / 2),
		static_cast<LONG>(Position.X + Size.X / 2),
		static_cast<LONG>(Position.Y + Size.Y / 2)
	};
}

bool UI::IsMouseInRectangle()
{
	RECT Rect = GetRectangle();
	POINT Mouse = InputManager::Get()->GetMousePosition();

	return ::PtInRect(&Rect, Mouse);
}
