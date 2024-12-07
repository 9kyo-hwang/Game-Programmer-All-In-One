#include "pch.h"
#include "GameplayScene.h"

#include "Monster.h"
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
	NewPlayer->SetPosition(Vector(400, 400));
	ObjectManager::Get()->Add(NewPlayer);

	for (int32 i = 0; i < 5; ++i)
	{
		Monster* NewMonster = ObjectManager::Get()->NewObject<Monster>();
		NewMonster->SetPosition(Vector{ static_cast<float>((i + 1) * 100), 100 });
		ObjectManager::Get()->Add(NewMonster);
	}
}

void GameplayScene::Update()
{
	const vector<Object*> Objects = ObjectManager::Get()->GetObjects();
	for (Object* Element : Objects)
	{
		Element->Update();
	}
}

void GameplayScene::Render(HDC InDC)
{
	const vector<Object*>& Objects = ObjectManager::Get()->GetObjects();
	for (Object* Element : Objects)
	{
		Element->Render(InDC);
	}
}
