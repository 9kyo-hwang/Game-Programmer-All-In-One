#pragma once

struct Tile
{
	// TODO
	int32 Value = 0;
	int32 Width = 64;
	int32 Height = 43;
	int32 SizeX = 48;
	int32 SizeY = 48;
};

class TileSet
{
public:
	TileSet();
	~TileSet();

	void Load(const wstring& Path);

	Vector2Int GetSize() const { return Size; }
	void SetSize(Vector2Int NewSize);

	int32 GetNumTile() const { return NumTile; }
	void SetNumTile(int32 InNumTile);

	Tile* GetTileAt(Vector2Int Cell);
	vector<vector<Tile>>& GetTiles() { return Tiles; }

private:
	Vector2Int Size{};
	int32 NumTile = 0;
	vector<vector<Tile>> Tiles{};
};

