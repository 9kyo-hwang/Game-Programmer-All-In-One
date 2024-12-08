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
	Monster* NewMonster = ObjectManager::Get()->NewObject<Monster>();
	NewMonster->SetPosition(Vector{ 400, 400 });
	ObjectManager::Get()->Add(NewMonster);
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
