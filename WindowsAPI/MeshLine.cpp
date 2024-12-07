#include "pch.h"
#include "MeshLine.h"
#include <fstream>

void MeshLine::Save(wstring Path)
{
	wofstream File(Path);

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

void MeshLine::Load(wstring Path)
{
	wifstream File(Path);
	if (!File.fail())
	{
		int32 Num; File >> Num;  // 라인 개수
		Lines.clear();

		for (int32 Index = 0; Index < Num; ++Index)
		{
			POINT From, To;

			wstring Line; File >> Line;
			::swscanf_s(Line.c_str(), L"(%d,%d)->(%d,%d)", &From.x, &From.y, &To.x, &To.y);

			Lines.push_back({ From, To });
		}
	}

	File.close();
}

void MeshLine::Render(HDC InDC, Vector Position) const
{
	// 3D의 경우 GPU에 Render를 위임할 수 있음(우리는 Mesh 정보만 넘겨주면 됨)
	for (const auto& [Point1, Point2] : Lines)
	{
		Vector From;
		From.X = static_cast<float>(Point1.x) + Position.X;  // 실제 플레이어 좌표 적용
		From.Y = static_cast<float>(Point1.y) + Position.Y;

		Vector To;
		To.X = static_cast<float>(Point2.x) + Position.X;
		To.Y = static_cast<float>(Point2.y) + Position.Y;

		Utils::DrawLine(InDC, From, To);
	}
}
