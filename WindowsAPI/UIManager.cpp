#include "pch.h"
#include "UIManager.h"
#include "InputManager.h"
#include "MeshLine.h"
#include "ResourceManager.h"

void UIManager::Initialize()
{	
	// 각종 테스트 설정
	WindPercent = 50;
	//_powerPercent = 50;
	StaminaPercent = 40;
	RemainTime = 7;
	PlayerAngle = 0.f;
	BarrelAngle = 20.f;
}

void UIManager::Render(HDC InDC)
{
	// UI
	RenderBackground(InDC);
	RenderWind(InDC);
	RenderPower(InDC);
	RenderStamina(InDC);
	RenderTime(InDC);
	RenderAngle(InDC);
	RenderWeaponChoice(InDC);
	RenderMiniMap(InDC);
}

void UIManager::RenderBackground(HDC InDC)
{
	if (const MeshLine* Mesh = ResourceManager::Get()->GetMeshLine(L"UI"))
	{
		Mesh->Render(InDC, Vector{ 0, 0 });
	}
}

void UIManager::RenderWind(HDC InDC)
{
	float minY = 560.0f / 600 * GWinSizeY;
	float maxY = 575.0f / 600 * GWinSizeY;
	float avgX = 100.0f / 800 * GWinSizeX;
	float sizeX = 49.0f / 800 * GWinSizeX;

	HBRUSH Brush = ::CreateSolidBrush(RGB(50, 198, 74));
	HBRUSH OldBrush = static_cast<HBRUSH>(::SelectObject(InDC, Brush));

	if (WindPercent < 0)
	{
		Rectangle(InDC
			, static_cast<int32>(avgX + sizeX * WindPercent / 100)
			, static_cast<int32>(minY)
			, static_cast<int32>(avgX)
			, static_cast<int32>(maxY));
	}
	else
	{
		Rectangle(InDC
			, static_cast<int32>(avgX)
			, static_cast<int32>(minY)
			, static_cast<int32>(avgX + sizeX * WindPercent / 100)
			, static_cast<int32>(maxY));
	}

	::SelectObject(InDC, OldBrush);
	::DeleteObject(Brush);
}

void UIManager::RenderPower(HDC InDC)
{
	{
		RECT Rect
		{
			.left = static_cast<LONG>(265.0f / 800 * GWinSizeX),
			.top = static_cast<LONG>(505.0f / 600 * GWinSizeY),
			.right = static_cast<LONG>(680.0f / 800 * GWinSizeX),
			.bottom = static_cast<LONG>(535.0f / 600 * GWinSizeY)
		};
		::Rectangle(InDC, Rect.left, Rect.top, Rect.right, Rect.bottom);
	}

	{
		RECT Rect
		{
			.left = static_cast<LONG>(270.0f / 800 * GWinSizeX),
			.top = static_cast<LONG>(510.0f / 600 * GWinSizeY),
			.right = static_cast<LONG>(270.0f / 800 * GWinSizeX + PowerPercent * 4),
			.bottom = static_cast<LONG>(530.0f / 600 * GWinSizeY)
		};

		HBRUSH Brush = ::CreateSolidBrush(RGB(255, 216, 216));
		HBRUSH OldBrush = static_cast<HBRUSH>(::SelectObject(InDC, Brush));

		::Rectangle(InDC, Rect.left, Rect.top, Rect.right, Rect.bottom);
		::SelectObject(InDC, OldBrush);
		::DeleteObject(Brush);
	}
}

void UIManager::RenderStamina(HDC InDC)
{
	{
		RECT Rect
		{
			.left = static_cast<LONG>(265.0f / 800 * GWinSizeX),
			.top = static_cast<LONG>(538.0f / 600 * GWinSizeY),
			.right = static_cast<LONG>(680.0f / 800 * GWinSizeX),
			.bottom = static_cast<LONG>(568.0f / 600 * GWinSizeY)
		};
		::Rectangle(InDC, Rect.left, Rect.top, Rect.right, Rect.bottom);
	}
	{
		RECT Rect
		{
			.left = static_cast<long>(270.0f / 800 * GWinSizeX),
			.top = static_cast<long>(543.0f / 600 * GWinSizeY),
			.right = static_cast<long>(270.0f / 800 * GWinSizeX + StaminaPercent * 4),
			.bottom = static_cast<long>(563.0f / 600 * GWinSizeY)
		};

		HBRUSH Brush = ::CreateSolidBrush(RGB(250, 236, 197));
		HBRUSH OldBrush = static_cast<HBRUSH>(::SelectObject(InDC, Brush));

		::Rectangle(InDC, Rect.left, Rect.top, Rect.right, Rect.bottom);
		::SelectObject(InDC, OldBrush);
		::DeleteObject(Brush);
	}
}

void UIManager::RenderTime(HDC InDC)
{
	HFONT Font = ::CreateFont(30, 0, 0, 0, 0,
		0, 0, 0, DEFAULT_CHARSET, 0,
		0, 0, 0, reinterpret_cast<LPCWSTR>("굴림체"));

	HFONT OldFont = static_cast<HFONT>(::SelectObject(InDC, Font));

	WCHAR message[100];
	::wsprintf(message, L"%02d", RemainTime);
	::TextOut(InDC, 728, 510, message, ::lstrlen(message));

	//wstring Text = std::format(L"{0}", RemainTime);
	//Utils::DrawTextW(InDC, { 728, 510 }, Text);

	::SelectObject(InDC, OldFont);
	::DeleteObject(Font);
}

void UIManager::RenderAngle(HDC InDC)
{
	::MoveToEx(InDC, 96, 520, nullptr);
	::Ellipse(InDC, 96 - 35, 520 - 35, 96 + 35, 520 + 35);

	::MoveToEx(InDC, 96, 520, nullptr);
	::LineTo(InDC
		, static_cast<int32>(96 + 30 * ::cos(PlayerAngle * PI / 180))
		, static_cast<int32>(520 - 30 * ::sin(PlayerAngle * PI / 180)));

	HPEN Pen = ::CreatePen(PS_SOLID, 0, RGB(204, 61, 61));
	HPEN OldPen = static_cast<HPEN>(::SelectObject(InDC, Pen));

	::MoveToEx(InDC, 96, 520, nullptr);
	::LineTo(InDC
		, static_cast<int32>(96 + 30 * ::cos(BarrelAngle * PI / 180))
		, static_cast<int32>(520 + -30 * ::sin(BarrelAngle * PI / 180)));

	::SelectObject(InDC, OldPen);
	::DeleteObject(Pen);
}

void UIManager::RenderWeaponChoice(HDC InDC)
{
	HBRUSH Brush = ::CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH OldBrush = static_cast<HBRUSH>(::SelectObject(InDC, Brush));

	if (!SpecialWeapon)
	{
		::Rectangle(InDC, 20 - 5, 550 - 5, 20 + 5, 550 + 5);
	}
	else
	{
		::Rectangle(InDC, 170 - 5, 550 - 5, 170 + 5, 550 + 5);
	}

	::SelectObject(InDC, OldBrush);
	::DeleteObject(Brush);
}

void UIManager::RenderMiniMap(HDC InDC)
{
	float ratioX = static_cast<float>(GMinimapSizeX) / static_cast<float>(1280);
	float ratioY = static_cast<float>(GMinimapSizeY) / static_cast<float>(720);

	// 미니맵 범위
	RECT Rect
	{
		GWinSizeX - GMinimapSizeX - 10,
		10,
		GWinSizeX - 10,
		10 + GMinimapSizeY
	};

	// 미니맵 테두리
	::Rectangle(InDC, Rect.left, Rect.top, Rect.right, Rect.bottom);
}