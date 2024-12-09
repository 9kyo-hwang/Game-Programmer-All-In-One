#include "pch.h"
#include "GameplayScene.h"

#include "ObjectManager.h"
#include "Player.h"
#include "TimerManager.h"
#include "UIManager.h"

GameplayScene::GameplayScene()
{
}

GameplayScene::~GameplayScene()
{
}

void GameplayScene::Initialize()
{
	// UI�� Manager�� �� ��� �ִ� ���� ���� �����ΰ�?
	// Sceneó�� UIBase�� �ϳ� �� ��, �̸� ��ӹ޾Ƽ� �װ͵��� �ε��ϴ� ���� ������ ������?
	UIManager::Get()->Initialize();

	{
		Player* NewPlayer = ObjectManager::Get()->NewObject<Player>();
		NewPlayer->SetType(EPlayerType::MissileTank);
		NewPlayer->SetPosition({ 100, 400 });
		NewPlayer->SetId(0);
		NewPlayer->SetLocalPlayer(true);  // �׷� �� ������ ���� �ؾ� ����...?

		ObjectManager::Get()->Add(NewPlayer);
	}

	{
		Player* NewPlayer = ObjectManager::Get()->NewObject<Player>();
		NewPlayer->SetType(EPlayerType::CanonTank);
		NewPlayer->SetPosition({ 700, 400 });
		NewPlayer->SetId(1);
		NewPlayer->SetLocalPlayer(false);

		ObjectManager::Get()->Add(NewPlayer);
	}
}

void GameplayScene::Update()
{
	float DeltaTime = TimerManager::Get()->GetDeltaTime();

	const vector<Object*> Objects = ObjectManager::Get()->GetObjects();  // Copy
	for (Object* Element : Objects)
	{
		Element->Update();
	}
}

void GameplayScene::Render(HDC InDC)
{
	UIManager::Get()->Render(InDC);

	const vector<Object*> Objects = ObjectManager::Get()->GetObjects();  // Copy
	for (Object* Element : Objects)
	{
		Element->Render(InDC);
	}
}
