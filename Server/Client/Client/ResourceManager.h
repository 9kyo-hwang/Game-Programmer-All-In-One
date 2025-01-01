#pragma once
#include "Singleton.h"

class Texture;
class Sprite;
class Flipbook;
class Tilemap;
class Sound;

class ResourceManager : public Singleton<ResourceManager>
{
public:
	~ResourceManager() override;
	void Initialize(HWND InWindowHandle, fs::path InResourcePath);
	void Clear();

	const fs::path& ResourceDir() { return ResourcePath; }

	Texture* GetTexture(const wstring& Name) { return Textures[Name]; }
	Texture* LoadTexture(const wstring& Name, const wstring& Path, uint32 Transparent = RGB(255, 0, 255));

	Sprite* GetSprite(const wstring& Name) { return Sprites[Name]; }
	Sprite* CreateSprite(const wstring& Name, Texture* BaseTexture, int32 X = 0, int32 Y = 0, int32 Width = 0, int32 Height = 0);

	Flipbook* GetFlipbook(const wstring& Name) { return Flipbooks[Name]; }
	Flipbook* CreateFlipbook(const wstring& Name);

	Tilemap* GetTilemap(const wstring& Name) { return Tilemaps[Name]; }
	Tilemap* CreateTilemap(const wstring& Name);
	void SaveTilemap(const wstring& Name, const wstring& Path);
	Tilemap* LoadTilemap(const wstring& Name, const wstring& Path);

	Sound* GetSound(const wstring& Name) { return Sounds[Name]; }
	Sound* LoadSound(const wstring& Name, const wstring& Path);

private:
	HWND WindowHandle{};
	fs::path ResourcePath{};
	unordered_map<wstring, Texture*> Textures;
	unordered_map<wstring, Sprite*> Sprites;
	unordered_map<wstring, Flipbook*> Flipbooks;
	unordered_map<wstring, Tilemap*> Tilemaps;
	unordered_map<wstring, Sound*> Sounds;
};

