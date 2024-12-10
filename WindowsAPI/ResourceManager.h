#pragma once
#include "Singleton.h"

class Texture;
class Sprite;

class ResourceManager : public Singleton<ResourceManager>
{
public:
	~ResourceManager();

	void Initialize(HWND InWindow, fs::path InResourcePath);
	void Clear();

	const fs::path& ResourceDir() { return ResourcePath; }

	Texture* GetTexture(const wstring& Name) { return Textures[Name]; }
	Texture* LoadTexture(const wstring& Name, const wstring& Path, uint32 Transparent = RGB(255, 0, 255));

	Sprite* GetSprite(const wstring& Name) { return Sprites[Name]; }
	Sprite* CreateSprite(const wstring& Name, Texture* BaseTexture, int32 X = 0, int32 Y = 0, int32 Width = 0, int32 Height = 0);

private:
	HWND Window{};
	fs::path ResourcePath{};
	unordered_map<wstring, Texture*> Textures;
	unordered_map<wstring, Sprite*> Sprites;
};

