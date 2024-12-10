#include "pch.h"
#include "DevelopmentScene.h"

#include "ASpriteActor.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "APlayer.h"
#include "Flipbook.h"

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
	Texture* Start = ResourceManager::Get()->LoadTexture(L"Start", L"Sprite\\UI\\Start.bmp");
	Texture* Edit = ResourceManager::Get()->LoadTexture(L"Edit", L"Sprite\\UI\\Edit.bmp");
	Texture* Exit = ResourceManager::Get()->LoadTexture(L"Exit", L"Sprite\\UI\\Exit.bmp"); 

	Sprite* BackgroundSprite = ResourceManager::Get()->CreateSprite(L"Stage01", Stage01);
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
		ASpriteActor* NewBackground = new ASpriteActor();
		NewBackground->SetSprite(BackgroundSprite);
		NewBackground->SetPosition({ BackgroundSprite->GetSize().X / 2, BackgroundSprite->GetSize().Y / 2 });
		Actors.push_back(NewBackground);
	}
	{
		APlayer* NewPlayer = new APlayer();
		Actors.push_back(NewPlayer);
	}

	for (AActor* Actor : Actors)
	{
		Actor->BeginPlay();
	}
}

void DevelopmentScene::Update()
{
	//float DeltaTime = TimerManager::Get()->GetDeltaTime();

	for (AActor* Actor : Actors)
	{
		Actor->Tick();
	}
}

void DevelopmentScene::Render(HDC InDC)
{
	for (AActor* Actor : Actors)
	{
		Actor->Render(InDC);
	}

	//Texture* Stage01 = ResourceManager::Get()->GetTexture(L"Stage01");
	//Sprite* StartOn = ResourceManager::Get()->GetSprite(L"Start_On");

	//// (0, 0) 지점에 (800, 600) 만큼 그리기. 단 Src의 (x, y) 좌표부터 시작
	//::BitBlt(InDC, 0, 0, GWinSizeX, GWinSizeY,
	//	StartOn->GetDC(), StartOn->GetPosition().X, StartOn->GetPosition().Y, SRCCOPY);
}
