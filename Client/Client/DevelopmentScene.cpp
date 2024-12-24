#include "pch.h"
#include "DevelopmentScene.h"
#include "ASpriteActor.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "APlayer.h"
#include "Flipbook.h"
#include "Tilemap.h"
#include "TilemapActor.h"
#include "Sound.h"

DevelopmentScene::DevelopmentScene()
{
}

DevelopmentScene::~DevelopmentScene()
{
	
}

void DevelopmentScene::Initialize()
{
	Texture* Stage01 = ResourceManager::Get()->LoadTexture(L"Stage01", L"Sprite\\Map\\Stage01.bmp");
	Texture* Tile = ResourceManager::Get()->LoadTexture(L"Tile", L"Sprite\\Map\\Tile.bmp", RGB(128, 128, 128));
	Texture* Start = ResourceManager::Get()->LoadTexture(L"Start", L"Sprite\\UI\\Start.bmp");
	Texture* Edit = ResourceManager::Get()->LoadTexture(L"Edit", L"Sprite\\UI\\Edit.bmp");
	Texture* Exit = ResourceManager::Get()->LoadTexture(L"Exit", L"Sprite\\UI\\Exit.bmp"); 

	Sprite* Stage01Sprite = ResourceManager::Get()->CreateSprite(L"Stage01", Stage01);
	ResourceManager::Get()->CreateSprite(L"Tile_O", Tile, 0, 0, 48, 48);
	ResourceManager::Get()->CreateSprite(L"Tile_X", Tile, 48, 0, 48, 48);
	ResourceManager::Get()->CreateSprite(L"Start_Off", Start, 0, 0, 150, 150);
	ResourceManager::Get()->CreateSprite(L"Start_On", Start, 150, 0, 150, 150);
	ResourceManager::Get()->CreateSprite(L"Edit_Off", Edit, 0, 0, 150, 150);
	ResourceManager::Get()->CreateSprite(L"Edit_On", Edit, 150, 0, 150, 150);
	ResourceManager::Get()->CreateSprite(L"Exit_Off", Exit, 0, 0, 150, 150);
	ResourceManager::Get()->CreateSprite(L"Exit_On", Exit, 150, 0, 150, 150);

	Texture* PlayerDown = ResourceManager::Get()->LoadTexture(L"PlayerDown", L"Sprite\\Player\\PlayerDown.bmp", RGB(128, 128, 128));
	Texture* PlayerUp = ResourceManager::Get()->LoadTexture(L"PlayerUp", L"Sprite\\Player\\PlayerUp.bmp", RGB(128, 128, 128));
	Texture* PlayerLeft = ResourceManager::Get()->LoadTexture(L"PlayerLeft", L"Sprite\\Player\\PlayerLeft.bmp", RGB(128, 128, 128));
	Texture* PlayerRight = ResourceManager::Get()->LoadTexture(L"PlayerRight", L"Sprite\\Player\\PlayerRight.bmp", RGB(128, 128, 128));
	ResourceManager::Get()->LoadTexture(L"Sword", L"Sprite\\Item\\Sword.bmp");
	//ResourceManager::Get()->LoadTexture(L"Potion", L"Sprite\\UI\\Potion.bmp");

	// PlayerIdle
	{
		Flipbook* FB_Up = ResourceManager::Get()->CreateFlipbook(L"FB_IdleUp");
		Flipbook* FB_Down = ResourceManager::Get()->CreateFlipbook(L"FB_IdleDown");
		Flipbook* FB_Left = ResourceManager::Get()->CreateFlipbook(L"FB_IdleLeft");
		Flipbook* FB_Right = ResourceManager::Get()->CreateFlipbook(L"FB_IdleRight");

		FB_Up->SetInfo({ PlayerUp, L"FB_IdleUp", {200, 200}, 0, 9, 0, 0.5f, true });
		FB_Down->SetInfo({ PlayerDown, L"FB_IdleDown", {200, 200}, 0, 9, 0, 0.5f, true });
		FB_Left->SetInfo({ PlayerLeft, L"FB_IdleLeft", {200, 200}, 0, 9, 0, 0.5f, true });
		FB_Right->SetInfo({ PlayerRight, L"FB_IdleRight", {200, 200}, 0, 9, 0, 0.5f, true });
	}
	// PlayerMove
	{
		Flipbook* FB_Up = ResourceManager::Get()->CreateFlipbook(L"FB_MoveUp");
		Flipbook* FB_Down = ResourceManager::Get()->CreateFlipbook(L"FB_MoveDown");
		Flipbook* FB_Left = ResourceManager::Get()->CreateFlipbook(L"FB_MoveLeft");
		Flipbook* FB_Right = ResourceManager::Get()->CreateFlipbook(L"FB_MoveRight");

		FB_Up->SetInfo({ PlayerUp, L"FB_MoveUp", {200, 200}, 0, 9, 1, 0.5f, true });
		FB_Down->SetInfo({ PlayerDown, L"FB_MoveDown", {200, 200}, 0, 9, 1, 0.5f, true });
		FB_Left->SetInfo({ PlayerLeft, L"FB_MoveLeft", {200, 200}, 0, 9, 1, 0.5f, true });
		FB_Right->SetInfo({ PlayerRight, L"FB_MoveRight", {200, 200}, 0, 9, 1, 0.5f, true });
	}
	// PlayerAttack
	{
		Flipbook* FB_Up = ResourceManager::Get()->CreateFlipbook(L"FB_AttackUp");
		Flipbook* FB_Down = ResourceManager::Get()->CreateFlipbook(L"FB_AttackDown");
		Flipbook* FB_Left = ResourceManager::Get()->CreateFlipbook(L"FB_AttackLeft");
		Flipbook* FB_Right = ResourceManager::Get()->CreateFlipbook(L"FB_AttackRight");

		FB_Up->SetInfo({ PlayerUp, L"FB_AttackUp", {200, 200}, 0, 7, 3, 0.5f, true });
		FB_Down->SetInfo({ PlayerDown, L"FB_AttackDown", {200, 200}, 0, 7, 3, 0.5f, true });
		FB_Left->SetInfo({ PlayerLeft, L"FB_AttackLeft", {200, 200}, 0, 7, 3, 0.5f, true });
		FB_Right->SetInfo({ PlayerRight, L"FB_AttackRight", {200, 200}, 0, 7, 3, 0.5f, true });
	}
	{
		AddActor(new APlayer());
	}

	{
		ASprite* Stage01SpriteActor = new ASprite();
		Stage01SpriteActor->SetSprite(Stage01Sprite);
		Stage01SpriteActor->SetLayer(ERenderLayer::Background);
		const Vector2Int Stage01SpriteSize = Stage01Sprite->GetSize();
		Stage01SpriteActor->SetCurrentPosition(Vector2(Stage01SpriteSize.X / 2, Stage01SpriteSize.Y / 2));

		AddActor(Stage01SpriteActor);
	}

	{
		// 충돌 처리 등을 수행할 특수한 액터 -> 캐싱
		TilemapActor* NewTilemapActor = new TilemapActor();
		AddActor(NewTilemapActor);
		ActiveTilemapActor = NewTilemapActor;
		{
			Tilemap* Tilemap01 = ResourceManager::Get()->CreateTilemap(L"Tilemap_01");
			Tilemap01->SetMapSize({ 63, 43 });
			Tilemap01->SetTileSize(48);

			ResourceManager::Get()->LoadTilemap(L"Tilemap_01", L"Tilemap\\Tilemap01.txt");

			ActiveTilemapActor->SetTilemap(Tilemap01);
			ActiveTilemapActor->SetShowDebug(false);
		}
	}

	//ResourceManager::Get()->LoadSound(L"BGM", L"Sound\\BGM.wav");
	//{
	//	// 현재 방식은 하나의 리소스를 돌려쓰는 방식
	//	// A에서 재생시킨 BGM이 다 끝나지 않았을 때 B가 BGM을 다시 재생하면 중간에 강제로 끊고 다시 재생
	//	// FMOD 같은 라이브러리는 이를 해결하기 위해 "채널링" 개념 도입

	//	Sound* BGM = ResourceManager::Get()->GetSound(L"BGM");
	//	BGM->Play(true);
	//}

	//ResourceManager::Get()->LoadSound(L"Attack", L"Sound\\Sword.wav");

	Super::Initialize();
}

void DevelopmentScene::Update(float DeltaTime)
{
	Super::Update();
}

void DevelopmentScene::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

bool DevelopmentScene::CanMoveTo(Vector2Int Dest) const
{
	assert(ActiveTilemapActor != nullptr);

	Tilemap* ActiveTilemap = ActiveTilemapActor->GetTilemap();
	assert(ActiveTilemap != nullptr);

	Tile* DestTile = ActiveTilemap->GetTileAt(Dest);
	assert(DestTile != nullptr);

	return DestTile->Value != 1;

	// 추후 몬스터가 추가되면 여기서 추가로 몬스터 위치를 확인
}

Vector2 DevelopmentScene::CellToWorld(Vector2Int Cell) const
{
	assert(ActiveTilemapActor != nullptr);

	Tilemap* ActiveTilemap = ActiveTilemapActor->GetTilemap();
	assert(ActiveTilemap != nullptr);

	int32 NumTile = ActiveTilemap->GetNumTile();
	Vector2 TilemapPosition = ActiveTilemapActor->GetCurrentPosition();

	return
	{
		TilemapPosition.X + Cell.X * NumTile + NumTile / 2,
		TilemapPosition.Y + Cell.Y * NumTile + NumTile / 2
	};
}

