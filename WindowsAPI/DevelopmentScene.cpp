#include "pch.h"
#include "DevelopmentScene.h"
#include "ASpriteActor.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "APlayer.h"
#include "CollisionManager.h"
#include "Flipbook.h"
#include "InputManager.h"
#include "Tilemap.h"
#include "TilemapActor.h"
#include "Sound.h"
#include "BoxCollider.h"

DevelopmentScene::DevelopmentScene()
{
}

DevelopmentScene::~DevelopmentScene()
{
	
}

void DevelopmentScene::Initialize()
{
	// 씬 마다 사용하는 리소스가 다름
	ResourceManager::Get()->LoadTexture(L"PlayerDown", L"Sprite\\Player\\PlayerDown.bmp", RGB(128, 128, 128));
	ResourceManager::Get()->LoadTexture(L"PlayerUp", L"Sprite\\Player\\PlayerUp.bmp", RGB(128, 128, 128));
	ResourceManager::Get()->LoadTexture(L"PlayerLeft", L"Sprite\\Player\\PlayerLeft.bmp", RGB(128, 128, 128));
	ResourceManager::Get()->LoadTexture(L"PlayerRight", L"Sprite\\Player\\PlayerRight.bmp", RGB(128, 128, 128));
	//ResourceManager::Get()->LoadTexture(L"Sword", L"Sprite\\Item\\Sword.bmp");
	//ResourceManager::Get()->LoadTexture(L"Potion", L"Sprite\\UI\\Potion.bmp");
	Texture* Stage01 = ResourceManager::Get()->LoadTexture(L"Stage01", L"Sprite\\Map\\Stage01.bmp");
	Texture* Tile = ResourceManager::Get()->LoadTexture(L"Tile", L"Sprite\\Map\\Tile.bmp", RGB(128, 128, 128));
	Texture* Start = ResourceManager::Get()->LoadTexture(L"Start", L"Sprite\\UI\\Start.bmp");
	Texture* Edit = ResourceManager::Get()->LoadTexture(L"Edit", L"Sprite\\UI\\Edit.bmp");
	Texture* Exit = ResourceManager::Get()->LoadTexture(L"Exit", L"Sprite\\UI\\Exit.bmp"); 

	Sprite* BackgroundSprite = ResourceManager::Get()->CreateSprite(L"Stage01", Stage01);
	Sprite* TileO = ResourceManager::Get()->CreateSprite(L"Tile_O", Tile, 0, 0, 48, 48);
	Sprite* TileX = ResourceManager::Get()->CreateSprite(L"Tile_X", Tile, 48, 0, 48, 48);
	ResourceManager::Get()->CreateSprite(L"Start_Off", Start, 0, 0, 150, 150);
	ResourceManager::Get()->CreateSprite(L"Start_On", Start, 150, 0, 150, 150);
	ResourceManager::Get()->CreateSprite(L"Edit_Off", Edit, 0, 0, 150, 150);
	ResourceManager::Get()->CreateSprite(L"Edit_On", Edit, 150, 0, 150, 150);
	ResourceManager::Get()->CreateSprite(L"Exit_Off", Exit, 0, 0, 150, 150);
	ResourceManager::Get()->CreateSprite(L"Exit_On", Exit, 150, 0, 150, 150);

	{
		Texture* PlayerUp = ResourceManager::Get()->GetTexture(L"PlayerUp");
		Flipbook* FB_MoveUp = ResourceManager::Get()->CreateFlipbook(L"FB_MoveUp");
		FB_MoveUp->SetInfo({ PlayerUp, L"FB_MoveUp", {200, 200}, 0, 9, 1, 0.5f, true });
	}
	{
		Texture* PlayerDown = ResourceManager::Get()->GetTexture(L"PlayerDown");
		Flipbook* FB_MoveDown = ResourceManager::Get()->CreateFlipbook(L"FB_MoveDown");
		FB_MoveDown->SetInfo({ PlayerDown, L"FB_MoveDown", {200, 200}, 0, 9, 1, 0.5f, true });
	}
	{
		Texture* PlayerLeft = ResourceManager::Get()->GetTexture(L"PlayerLeft");
		Flipbook* FB_MoveLeft = ResourceManager::Get()->CreateFlipbook(L"FB_MoveLeft");
		FB_MoveLeft->SetInfo({ PlayerLeft, L"FB_MoveLeft", {200, 200}, 0, 9, 1, 0.5f, true });
	}
	{
		Texture* PlayerRight = ResourceManager::Get()->GetTexture(L"PlayerRight");
		Flipbook* FB_MoveRight = ResourceManager::Get()->CreateFlipbook(L"FB_MoveRight");
		FB_MoveRight->SetInfo({ PlayerRight, L"FB_MoveRight", {200, 200}, 0, 9, 1, 0.5f, true });
	}

	{
		ASpriteActor* Background = new ASpriteActor();
		Background->SetSprite(BackgroundSprite);
		Background->SetPosition({ static_cast<float>(BackgroundSprite->GetSize().X / 2), static_cast<float>(BackgroundSprite->GetSize().Y / 2) });
		Background->SetLayer(ERenderLayer::Background);

		AddActor(Background);
	}
	{
		APlayer* Player = new APlayer();

		BoxCollider* Collider = new BoxCollider();
		Collider->Size = {64, 64};
		Collider->SetCollideLayer(ECollideLayer::Object);
		Collider->AddCollisionFlagLayer(ECollideLayer::Ground);
		CollisionManager::Get()->AddCollider(Collider);  // 임시로 하드 코딩
		Player->AddComponent(Collider);

		AddActor(Player);
	}
	{
		AActor* Actor = new AActor();
		Actor->SetLayer(ERenderLayer::Object);
		Actor->SetPosition({ 200, 400 });

		BoxCollider* Collider = new BoxCollider();
		Collider->Size = { 10000, 100 };
		Collider->SetCollideLayer(ECollideLayer::Ground);
		CollisionManager::Get()->AddCollider(Collider);  // Actor의 AddComponent에서 수행하는 게 가장 적합할 듯...?
		Actor->AddComponent(Collider);

		AddActor(Actor);
	}
	{
		// 충돌 처리 등을 수행할 특수한 액터 -> 캐싱
		TilemapActor* NewTilemapActor = new TilemapActor();
		AddActor(NewTilemapActor);
		MyTilemapActor = NewTilemapActor;
		{
			Tilemap* Tilemap01 = ResourceManager::Get()->CreateTilemap(L"Tilemap_01");
			Tilemap01->SetMapSize({ 63, 43 });
			Tilemap01->SetTileSize(48);

			MyTilemapActor->SetTilemap(Tilemap01);
			MyTilemapActor->SetShowDebug(false);
		}
	}

	ResourceManager::Get()->LoadSound(L"BGM", L"Sound\\BGM.wav");
	{
		// 현재 방식은 하나의 리소스를 돌려쓰는 방식
		// A에서 재생시킨 BGM이 다 끝나지 않았을 때 B가 BGM을 다시 재생하면 중간에 강제로 끊고 다시 재생
		// FMOD 같은 라이브러리는 이를 해결하기 위해 "채널링" 개념 도입

		//Sound* BGM = ResourceManager::Get()->GetSound(L"BGM");
		//BGM->Play(true);  // 검 휘두르기 같은 BGM은 loop을 false로 두면 됨
		//SoundManager::Get()->Play(L"BGM");  // 위와 동일한 기능
	}

	Super::Initialize();
}

void DevelopmentScene::Update(float DeltaTime)
{
	Super::Update();

	if (InputManager::Get()->GetButtonDown(EKeyCode::Q))
	{
		ResourceManager::Get()->SaveTilemap(L"Tilemap_01", L"Tilemap\\Tilemap01.txt");
	}
	else if (InputManager::Get()->GetButtonDown(EKeyCode::E))
	{
		ResourceManager::Get()->LoadTilemap(L"Tilemap_01", L"Tilemap\\Tilemap01.txt");
	}
}

void DevelopmentScene::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

