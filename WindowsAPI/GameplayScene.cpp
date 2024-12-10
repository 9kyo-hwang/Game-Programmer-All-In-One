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

	// 초기화 기능이 포함돼있어 여기서도 한 번 호출
	// 단 0번 플레이어부터 시작하기 위해 Turn 초기값을 1로 세팅
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

	// 현재 이 방식은 단순히 정보를 띄워야 할 UIManager가 실제 데이터를 건드는 나쁜 방식
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
	// 추후 규칙에 따라 턴 변경 코드를 작성하면 됨

	Turn ^= 1;  // 현재는 2명의 플레이어 밖에 없으므로 0 <-> 1
	const vector<Object*> Objects = ObjectManager::Get()->GetObjects();  // 각 씬 별로 오브젝트 들고 있는 게 더 편할듯?
	for (Object* Element : Objects)
	{
		if (Element->GetObjectType() != EObjectType::Player)
		{
			continue;
		}

		Player* Opponent = static_cast<Player*>(Element);
		if (Opponent->GetId() == Turn)  // 2명밖에 없어서 Id == 턴
		{
			Opponent->SetLocalPlayer(true);  // 해당 턴의 플레이어가 내가 조작할 플레이어
		}
		else
		{
			Opponent->SetLocalPlayer(false);
		}
	}

	// 턴을 교체하므로 각종 수치 초기화
	UIManager::Get()->SetRemainTime(10);
	UIManager::Get()->SetStaminaPercent(100.0f);
	UIManager::Get()->SetPowerPercent(0.0f);
	UIManager::Get()->SetWindPercent(static_cast<float>(rand() % 200 - 100));
}
