#include "pch.h"
#include "EditorScene.h"
#include "InputManager.h"
#include <fstream>

EditorScene::EditorScene()
{
}

EditorScene::~EditorScene()
{
}

void EditorScene::Initialize()
{
}

void EditorScene::Update()
{
	if (InputManager::Get()->GetButtonDown(EKeyCode::LeftMouseButton))
	{
		POINT MousePosition = InputManager::Get()->GetMousePosition();
		if (bSetOrigin)  // 최초로 클릭했을 경우
		{
			LastPosition = MousePosition;
			bSetOrigin = false;
		}
		else
		{
			Lines.push_back({ LastPosition, MousePosition });
			LastPosition = MousePosition;
		}
	}

	// 한 붓 그리기가 끝나고 다른 선을 그리고 싶음을 의미
	if (InputManager::Get()->GetButtonDown(EKeyCode::RightMouseButton))
	{
		bSetOrigin = true;
	}

	// Save
	if (InputManager::Get()->GetButtonDown(EKeyCode::S))
	{
		wofstream File(L"Unit.txt");

		int32 MinX = INT32_MAX;
		int32 MaxX = INT32_MIN;
		int32 MinY = INT32_MAX;
		int32 MaxY = INT32_MIN;

		for (const auto& [From, To] : Lines)
		{
			MinX = min(min(MinX, From.x), To.x);
			MaxX = max(max(MaxX, From.x), To.x);
			MinY = min(min(MinY, From.y), To.y);
			MaxY = max(max(MaxY, From.y), To.y);
		}

		int32 MidX = (MinX + MaxX) / 2;
		int32 MidY = (MinY + MaxY) / 2;

		// 라인 개수
		File << static_cast<int32>(Lines.size()) << "\n";
		for (auto [From, To] : Lines)
		{
			From.x -= MidX;
			From.y -= MidY;
			To.x -= MidX;
			To.y -= MidY;

			File << std::format(L"({0},{1})->({2},{3})\n", From.x, From.y, To.x, To.y);
		}

		File.close();
	}

	// Load
	if (InputManager::Get()->GetButtonDown(EKeyCode::D))
	{
		wifstream File(L"Unit.txt");
		if (!File.fail())
		{
			int32 Num; File >> Num;  // 라인 개수
			Lines.clear();

			int32 MidX = 400;
			int32 MidY = 300;
			for (int32 Index = 0; Index < Num; ++Index)
			{
				POINT From, To;

				wstring Line; File >> Line;
				::swscanf_s(Line.c_str(), L"(%d,%d)->(%d,%d)", &From.x, &From.y, &To.x, &To.y);

				From.x += MidX;
				From.y += MidY;
				To.x += MidX;
				To.y += MidY;
				Lines.push_back({ From, To });
			}

			bSetOrigin = true;
		}

		File.close();
	}
}

void EditorScene::Render(HDC InDC)
{
	for (const auto& [Point1, Point2] : Lines)
	{
		Vector2 From;
		From.x = static_cast<float>(Point1.x);
		From.y = static_cast<float>(Point1.y);

		Vector2 To;
		To.x = static_cast<float>(Point2.x);
		To.y = static_cast<float>(Point2.y);

		Utils::DrawLine(InDC, From, To);
	}
}
