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
	// C++ 파일 입출력은 기본적으로 Text 포맷. 보안상 binary로 저장하는 것이 일반적
	// 인자로 Mode를 변경할 수 있음, 예) ios_base::binary
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

	// 맵을 새로 그려야할 경우 이곳에서 수행
}


Tile* Tilemap::GetTileAt(Vector2Int Position)
{
	assert(0 <= Position.X && Position.X < MapSize.X && 0 <= Position.Y && Position.Y < MapSize.Y);
	return &Tiles[Position.Y][Position.X];
}
