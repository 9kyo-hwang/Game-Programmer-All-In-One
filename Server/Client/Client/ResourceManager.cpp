#include "pch.h"
#include "ResourceManager.h"
#include <ranges>

#include "Flipbook.h"
#include "Sound.h"
#include "Texture.h"
#include "Sprite.h"
#include "Tilemap.h"

ResourceManager::~ResourceManager()
{
	Clear();
}

void ResourceManager::Initialize(HWND InWindowHandle, fs::path InResourcePath)
{
	WindowHandle = InWindowHandle;
	ResourcePath = InResourcePath;
}

void ResourceManager::Clear()
{
	for (Texture* CachedTexture : Textures | views::values)
	{
		SAFE_DELETE(CachedTexture);
	}

	Textures.clear();

	for (Sprite* CachedSprite : Sprites | views::values)
	{
		SAFE_DELETE(CachedSprite);
	}

	Sprites.clear();

	for (Flipbook* CachedFlipbook : Flipbooks | views::values)
	{
		SAFE_DELETE(CachedFlipbook);
	}

	Flipbooks.clear();

	for (Tilemap* CachedTilemap : Tilemaps | views::values)
	{
		SAFE_DELETE(CachedTilemap);
	}

	Tilemaps.clear();
}

Texture* ResourceManager::LoadTexture(const wstring& Name, const wstring& Path, uint32 Transparent)
{
	if (Textures.contains(Name))
	{
		return Textures[Name];
	}

	fs::path TexturePath = ResourcePath / Path;

	Texture* NewTexture = new Texture();
	NewTexture->LoadTexture(WindowHandle, TexturePath.wstring());
	NewTexture->SetTransparent(Transparent);

	return Textures[Name] = NewTexture;
}

Sprite* ResourceManager::CreateSprite(const wstring& Name, Texture* BaseTexture, int32 X, int32 Y, int32 Width,
	int32 Height)
{
	if (Sprites.contains(Name))
	{
		return Sprites[Name];
	}

	if (Width == 0)
	{
		Width = BaseTexture->GetSize().X;
	}
	if (Height == 0)
	{
		Height = BaseTexture->GetSize().Y;
	}

	Sprite* NewSprite = new Sprite(BaseTexture, X, Y, Width, Height);
	return Sprites[Name] = NewSprite;
}

Flipbook* ResourceManager::CreateFlipbook(const wstring& Name)
{
	if (Flipbooks.contains(Name))
	{
		return Flipbooks[Name];
	}

	Flipbook* NewFlipbook = new Flipbook();
	return Flipbooks[Name] = NewFlipbook;
}

Tilemap* ResourceManager::CreateTilemap(const wstring& Name)
{
	if (Tilemaps.contains(Name))
	{
		return Tilemaps[Name];
	}

	Tilemap* NewTilemap = new Tilemap();
	return Tilemaps[Name] = NewTilemap;
}

void ResourceManager::SaveTilemap(const wstring& Name, const wstring& Path)
{
	Tilemap* Target = GetTilemap(Name);
	fs::path FullPath = ResourcePath / Path;
	Target->Save(FullPath);
}

Tilemap* ResourceManager::LoadTilemap(const wstring& Name, const wstring& Path)
{
	Tilemap* Target = nullptr;
	if (!Tilemaps.contains(Name))
	{
		Tilemaps[Name] = new Tilemap();
	}

	Target = Tilemaps[Name];
	fs::path FullPath = ResourcePath / Path;
	Target->Load(FullPath);

	return Target;
}

Sound* ResourceManager::LoadSound(const wstring& Name, const wstring& Path)
{
	if (Sounds.contains(Name))
	{
		return Sounds[Name];
	}

	fs::path FullPath = ResourcePath / Path;
	Sound* NewSound = new Sound();
	NewSound->Load(FullPath);
	return Sounds[Name] = NewSound;
}
