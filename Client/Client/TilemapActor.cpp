#include "pch.h"
#include "TilemapActor.h"

#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "Tilemap.h"

ATilemap::ATilemap()
{
}

ATilemap::~ATilemap()
{
}

void ATilemap::BeginPlay()
{
	Super::BeginPlay();
}

void ATilemap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PickTile();  // 매 틱마다 감지
}

void ATilemap::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);

	if (!MyTilemap)
	{
		return;
	}

	if (!bShowDebug)
	{
		// 렌더링하지 않고, 메모리 상으로 들고만 있을 용도
		return;
	}

	const Vector2Int MapSize = MyTilemap->GetMapSize();

	auto& Tiles = MyTilemap->GetTiles();
	Sprite* TileO = ResourceManager::Get()->GetSprite(L"Tile_O");
	Sprite* TileX = ResourceManager::Get()->GetSprite(L"Tile_X");

	Vector2 Camera = SceneManager::Get()->GetCameraPosition();

	int32 FOV_LeftX = static_cast<int32>(Camera.X) - GWinSizeX / 2;
	int32 FOV_LeftY = static_cast<int32>(Camera.Y) - GWinSizeY / 2;
	int32 FOV_RightX = static_cast<int32>(Camera.X) + GWinSizeX / 2;
	int32 FOV_RightY = static_cast<int32>(Camera.Y) + GWinSizeY / 2;

	int32 BeginX = (FOV_LeftX - static_cast<int32>(CurrentPosition.X)) / TileO->GetSize().X;
	int32 BeginY = (FOV_LeftY - static_cast<int32>(CurrentPosition.Y)) / TileO->GetSize().Y;
	int32 EndX = (FOV_RightX - static_cast<int32>(CurrentPosition.X)) / TileO->GetSize().X;
	int32 EndY = (FOV_RightY - static_cast<int32>(CurrentPosition.Y)) / TileO->GetSize().Y;
		
	for (int32 y = BeginY; y < EndY; ++y)  // 왜 나는 하나 더 그려지지...?
	{
		for (int32 x = BeginX; x <= EndX; ++x)  // X는 똑같이 하나 부족하고...
		{
			if (x < 0 || x >= MapSize.X || y < 0 || y >= MapSize.Y)
			{
				continue;
			}

			switch (Tiles[y][x].Value)
			{
			case 0:  // O
				::TransparentBlt(DeviceContextHandle, 
					CurrentPosition.X + x * TileO->GetSize().X - (Camera.X - GWinSizeX / 2),  // 액터가 그리는 스프라이트의 "좌상단 꼭짓점"에 위치하도록 설정
					CurrentPosition.Y + y * TileO->GetSize().Y - (Camera.Y - GWinSizeY / 2),
					TileO->GetSize().X,
					TileO->GetSize().Y,
					TileO->GetDC(), 
					TileO->GetPosition().X, 
					TileO->GetPosition().Y,
					TileO->GetSize().X,
					TileO->GetSize().Y,
					TileO->GetTransparent());
				break;
			case 1:  // X
				::TransparentBlt(DeviceContextHandle,
					CurrentPosition.X + x * TileX->GetSize().X - (Camera.X - GWinSizeX / 2),
					CurrentPosition.Y + y * TileX->GetSize().Y - (Camera.Y - GWinSizeY / 2),
					TileX->GetSize().X,
					TileX->GetSize().Y,
					TileX->GetDC(),
					TileX->GetPosition().X,
					TileX->GetPosition().Y,
					TileX->GetSize().X,
					TileX->GetSize().Y,
					TileX->GetTransparent());
				break;
			default:
				break;
			}
		}
	}
}

void ATilemap::PickTile()
{
	if (InputManager::Get()->GetButtonDown(EKeyCode::LeftMouseButton))
	{
		Vector2 Camera = SceneManager::Get()->GetCameraPosition();
		int32 LeftTopX = Camera.X - GWinSizeX / 2;
		int32 LeftTopY = Camera.Y - GWinSizeY / 2;

		POINT Mouse = InputManager::Get()->GetMousePosition();
		int32 WorldX = Mouse.x + LeftTopX;
		int32 WorldY = Mouse.y + LeftTopY;

		int32 TileX = WorldX / MyTilemap->GetNumTile();
		int32 TileY = WorldY / MyTilemap->GetNumTile();

		if (Tile* TargetTile = MyTilemap->GetTileAt({ TileX, TileY }))
		{
			TargetTile->Value ^= 1;
		}
	}
}
