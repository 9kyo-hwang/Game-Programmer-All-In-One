#include "pch.h"
#include "Tilemap.h"

#include <cassert>
#include <fstream>

Tilemap::Tilemap()
{
}

Tilemap::~Tilemap()
{
}

void Tilemap::Load(const wstring& Path)
{
	// C++ ���� ������� �⺻������ Text ����. ���Ȼ� binary�� �����ϴ� ���� �Ϲ���
	// ���ڷ� Mode�� ������ �� ����, ��) ios_base::binary
	wifstream Stream(Path);

	Stream >> MapSize.X >> MapSize.Y;
	SetMapSize(MapSize);

	for (int32 y = 0; y < MapSize.Y; ++y)
	{
		wstring Line; Stream >> Line;
		for (int32 x = 0; x < MapSize.X; ++x)
		{
			Tiles[y][x].Value = Line[x] - L'0';
		}
	}

	Stream.close();
}

void Tilemap::Save(const wstring& Path)
{
	wofstream Stream(Path);

	Stream << MapSize.X << "\n" << MapSize.Y << "\n";
	for (int32 y = 0; y < MapSize.Y; ++y)
	{
		for (int32 x = 0; x < MapSize.X; ++x)
		{
			Stream << Tiles[y][x].Value;
		}
		Stream << "\n";
	}

	Stream.close();
}

void Tilemap::SetMapSize(Vector2Int NewMapSize)
{
	MapSize = NewMapSize;

	Tiles.assign(NewMapSize.Y, vector<Tile>(NewMapSize.X, Tile{}));
}

void Tilemap::SetTileSize(int32 NewTileSize)
{
	NumTile = NewTileSize;

	// ���� ���� �׷����� ��� �̰����� ����
}


Tile* Tilemap::GetTileAt(Vector2Int Cell)
{
	if (0 <= Cell.X && Cell.X < MapSize.X && 0 <= Cell.Y && Cell.Y < MapSize.Y)
	{
		return &Tiles[Cell.Y][Cell.X];
	}

	return nullptr;
}
