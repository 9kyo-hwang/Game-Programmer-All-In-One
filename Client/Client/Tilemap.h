#pragma once
#include "Resources.h"

struct Tile
{
	// TODO
	int32 Value = 0;
	int32 Width = 64;
	int32 Height = 43;
	int32 SizeX = 48;
	int32 SizeY = 48;
};

class Tilemap : public Resources
{
	using Super = Resources;

public:
	Tilemap();
	~Tilemap() override;

	void Load(const wstring& Path) override;
	void Save(const wstring& Path) override;

	Vector2Int GetMapSize() const { return MapSize; }
	void SetMapSize(Vector2Int NewMapSize);

	int32 GetNumTile() const { return NumTile; }
	void SetTileSize(int32 NewTileSize);

	Tile* GetTileAt(Vector2Int Cell);
	vector<vector<Tile>>& GetTiles() { return Tiles; }

private:
	Vector2Int MapSize{};  // (10, 10): 10개짜리가 10줄
	int32 NumTile = 0;
	vector<vector<Tile>> Tiles{};  // 2차원 좌표로 관리
};