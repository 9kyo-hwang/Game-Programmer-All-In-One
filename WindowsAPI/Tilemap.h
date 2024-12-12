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

	int32 GetTileSize() const { return TileSize; }
	void SetTileSize(int32 NewTileSize);

	Tile* GetTileAt(Vector2Int Position);
	vector<vector<Tile>>& GetTiles() { return Tiles; }

private:
	Vector2Int MapSize{};  // (10, 10): 10��¥���� 10��
	int32 TileSize = 0;
	vector<vector<Tile>> Tiles{};  // 2���� ��ǥ�� ����
};