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
	wifstream Fin(Path);

	Fin >> MapSize.X >> MapSize.Y;
	SetMapSize(MapSize);

	for (int32 y = 0; y < MapSize.Y; ++y)
	{
		for (int32 x = 0; x < MapSize.X; ++x)
		{
			Fin >> Tiles[y][x].Value;
		}
	}

	Fin.close();
}

void Tilemap::Save(const wstring& Path)
{
	wofstream Fout(Path);

	Fout << MapSize.X << "\n" << MapSize.Y << "\n";
	for (int32 y = 0; y < MapSize.Y; ++y)
	{
		for (int32 x = 0; x < MapSize.X; ++x)
		{
			Fout << Tiles[y][x].Value << " ";
		}
		Fout << "\n";
	}

	Fout.close();
}

void Tilemap::SetMapSize(Vector2Int NewMapSize)
{
	MapSize = NewMapSize;

	Tiles.assign(NewMapSize.Y, vector<Tile>(NewMapSize.X, Tile{}));
}

void Tilemap::SetTileSize(int32 NewTileSize)
{
	TileSize = NewTileSize;

	// ���� ���� �׷����� ��� �̰����� ����
}


Tile* Tilemap::GetTileAt(Vector2Int Position)
{
	assert(0 <= Position.X && Position.X < MapSize.X && 0 <= Position.Y && Position.Y < MapSize.Y);
	return &Tiles[Position.Y][Position.X];
}
