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

	// �ʱ�ȭ ����� ���Ե��־� ���⼭�� �� �� ȣ��
	// �� 0�� �÷��̾���� �����ϱ� ���� Turn �ʱⰪ�� 1�� ����
	FlipGameTurn();
}

void GameplayScene::Update()
{
	float DeltaTime = TimerManager::Get()->GetDeltaTime();

	const vector<Object*> Objects = ObjectManager::Get()->GetObjects();  // Copy
	for (Object* Element : Objects)
	{
		Element->Update();
	}

	// ���� �� ����� �ܼ��� ������ ����� �� UIManager�� ���� �����͸� �ǵ�� ���� ���
	ElapsedTime += DeltaTime;
	if (ElapsedTime >= 1.0f)
	{
		ElapsedTime = 0.0f;

		int32 Time = UIManager::Get()->GetRemainTime();
		Time = max(0, Time - 1);
		UIManager::Get()->SetRemainTime(Time);

		if (Time == 0)
		{
			FlipGameTurn();
		}
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

void GameplayScene::FlipGameTurn()
{
	// ���� ��Ģ�� ���� �� ���� �ڵ带 �ۼ��ϸ� ��

	Turn ^= 1;  // ����� 2���� �÷��̾� �ۿ� �����Ƿ� 0 <-> 1
	const vector<Object*> Objects = ObjectManager::Get()->GetObjects();  // �� �� ���� ������Ʈ ��� �ִ� �� �� ���ҵ�?
	for (Object* Element : Objects)
	{
		if (Element->GetObjectType() != EObjectType::Player)
		{
			continue;
		}

		Player* Opponent = static_cast<Player*>(Element);
		if (Opponent->GetId() == Turn)  // 2��ۿ� ��� Id == ��
		{
			Opponent->SetLocalPlayer(true);  // �ش� ���� �÷��̾ ���� ������ �÷��̾�
		}
		else
		{
			Opponent->SetLocalPlayer(false);
		}
	}

	// ���� ��ü�ϹǷ� ���� ��ġ �ʱ�ȭ
	UIManager::Get()->SetRemainTime(10);
	UIManager::Get()->SetStaminaPercent(100.0f);
	UIManager::Get()->SetPowerPercent(0.0f);
	UIManager::Get()->SetWindPercent(static_cast<float>(rand() % 200 - 100));
}
