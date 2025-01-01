#include "pch.h"
#include "TileSet.h"

#include <fstream>

TileSet::TileSet()
{
}

TileSet::~TileSet()
{
}

void TileSet::Load(const wstring& Path)
{
	wifstream Stream(Path);

	Stream >> Size.X >> Size.Y;
	SetSize(Size);

	for (int32 y = 0; y < Size.Y; ++y)
	{
		wstring Line; Stream >> Line;
		for (int32 x = 0; x < Size.X; ++x)
		{
			Tiles[y][x].Value = Line[x] - L'0';
		}
	}

	Stream.close();
}

void TileSet::SetSize(Vector2Int NewSize)
{
	Size = NewSize;
	Tiles.assign(NewSize.Y, vector<Tile>(NewSize.X, Tile{}));
}

void TileSet::SetNumTile(int32 InNumTile)
{
	NumTile = InNumTile;
}

Tile* TileSet::GetTileAt(Vector2Int Cell)
{
	if (0 <= Cell.X && Cell.X < Size.X && 0 <= Cell.Y && Cell.Y < Size.Y)
	{
		return &Tiles[Cell.Y][Cell.X];
	}

	return nullptr;
}
