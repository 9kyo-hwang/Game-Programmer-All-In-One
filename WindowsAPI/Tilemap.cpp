#include "pch.h"
#include "Tilemap.h"

#include <cassert>

Tilemap::Tilemap()
{
}

Tilemap::~Tilemap()
{
}

void Tilemap::Load(const wstring& Path)
{
	Super::Load(Path);
}

void Tilemap::Save(const wstring& Path)
{
	Super::Save(Path);
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
