#include "pch.h"
#include "ResourceManager.h"
#include <ranges>

#include "Flipbook.h"
#include "Texture.h"
#include "Sprite.h"

ResourceManager::~ResourceManager()
{
	Clear();
}

void ResourceManager::Initialize(HWND InWindow, fs::path InResourcePath)
{
	Window = InWindow;
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
}

Texture* ResourceManager::LoadTexture(const wstring& Name, const wstring& Path, uint32 Transparent)
{
	if (Textures.contains(Name))
	{
		return Textures[Name];
	}

	fs::path TexturePath = ResourcePath / Path;

	Texture* NewTexture = new Texture();
	NewTexture->Load(Window, TexturePath.wstring());
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
