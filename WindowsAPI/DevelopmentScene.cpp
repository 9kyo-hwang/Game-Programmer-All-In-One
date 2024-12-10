#include "pch.h"
#include "DevelopmentScene.h"
#include "TimerManager.h"
#include "ResourceManager.h"
#include "Sprite.h"

DevelopmentScene::DevelopmentScene()
{
}

DevelopmentScene::~DevelopmentScene()
{
}

void DevelopmentScene::Initialize()
{
	// 씬 마다 사용하는 리소스가 다름
	ResourceManager::Get()->LoadTexture(L"Stage01", L"Sprite\\Map\\Stage01.bmp");
	//ResourceManager::Get()->LoadTexture(L"Sword", L"Sprite\\Item\\Sword.bmp");
	//ResourceManager::Get()->LoadTexture(L"Potion", L"Sprite\\UI\\Potion.bmp");
	ResourceManager::Get()->LoadTexture(L"PlayerDown", L"Sprite\\Player\\PlayerDown.bmp", RGB(128, 128, 128));
	ResourceManager::Get()->LoadTexture(L"PlayerUp", L"Sprite\\Player\\PlayerUp.bmp", RGB(128, 128, 128));
	ResourceManager::Get()->LoadTexture(L"PlayerLeft", L"Sprite\\Player\\PlayerLeft.bmp", RGB(128, 128, 128));
	ResourceManager::Get()->LoadTexture(L"PlayerRight", L"Sprite\\Player\\PlayerRight.bmp", RGB(128, 128, 128));
	Texture* Start = ResourceManager::Get()->LoadTexture(L"Start", L"Sprite\\UI\\Start.bmp");
	ResourceManager::Get()->LoadTexture(L"Edit", L"Sprite\\UI\\Edit.bmp");
	ResourceManager::Get()->LoadTexture(L"Exit", L"Sprite\\UI\\Exit.bmp"); 

	ResourceManager::Get()->CreateSprite(L"Start_On", Start, 150, 0, 150, 150);
}

void DevelopmentScene::Update()
{
	//float DeltaTime = TimerManager::Get()->GetDeltaTime();
}

void DevelopmentScene::Render(HDC InDC)
{
	Texture* Stage01 = ResourceManager::Get()->GetTexture(L"Stage01");
	Sprite* StartOn = ResourceManager::Get()->GetSprite(L"Start_On");

	// (0, 0) 지점에 (800, 600) 만큼 그리기. 단 Src의 (x, y) 좌표부터 시작
	::BitBlt(InDC, 0, 0, GWinSizeX, GWinSizeY,
		StartOn->GetDC(), StartOn->GetPosition().X, StartOn->GetPosition().Y, SRCCOPY);
}
