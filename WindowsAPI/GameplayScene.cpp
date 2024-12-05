#include "pch.h"
#include "GameplayScene.h"

#include "ObjectManager.h"
#include "Player.h"

GameplayScene::GameplayScene()
{
}

GameplayScene::~GameplayScene()
{
}

void GameplayScene::Initialize()
{
	// 일단은 플레이어만
	Player* NewPlayer = ObjectManager::Get()->NewObject<Player>();
	NewPlayer->SetPosition(Vector2(400, 400));
	ObjectManager::Get()->Add(NewPlayer);
}

void GameplayScene::Update()
{
	for (Object* Element : ObjectManager::Get()->GetObjects())
	{
		Element->Update();
	}
}

void GameplayScene::Render(HDC InDC)
{
	for (Object* Element : ObjectManager::Get()->GetObjects())
	{
		Element->Render(InDC);
	}
}
