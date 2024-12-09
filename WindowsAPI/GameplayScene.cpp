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
	// UI를 Manager가 다 들고 있는 것이 좋은 구조인가?
	// Scene처럼 UIBase를 하나 둔 뒤, 이를 상속받아서 그것들을 로드하는 식이 괜찮지 않을까?
	UIManager::Get()->Initialize();

	{
		Player* NewPlayer = ObjectManager::Get()->NewObject<Player>();
		NewPlayer->SetType(EPlayerType::MissileTank);
		NewPlayer->SetPosition({ 100, 400 });
		NewPlayer->SetId(0);
		NewPlayer->SetLocalPlayer(true);  // 그럼 턴 관리는 누가 해야 하지...?

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
