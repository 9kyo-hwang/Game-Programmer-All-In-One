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

	PickTile();  // �� ƽ���� ����
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
		// ���������� �ʰ�, �޸� ������ ��� ���� �뵵
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
		
	for (int32 y = BeginY; y < EndY; ++y)  // �� ���� �ϳ� �� �׷�����...?
	{
		for (int32 x = BeginX; x <= EndX; ++x)  // X�� �Ȱ��� �ϳ� �����ϰ�...
		{
			if (x < 0 || x >= MapSize.X || y < 0 || y >= MapSize.Y)
			{
				continue;
			}

			switch (Tiles[y][x].Value)
			{
			case 0:  // O
				::TransparentBlt(DeviceContextHandle, 
					CurrentPosition.X + x * TileO->GetSize().X - (Camera.X - GWinSizeX / 2),  // ���Ͱ� �׸��� ��������Ʈ�� "�»�� ������"�� ��ġ�ϵ��� ����
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
