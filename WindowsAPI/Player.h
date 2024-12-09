#pragma once
#include "Object.h"

enum class EPlayerType : uint8
{
	CanonTank,
	MissileTank,
};

class Player final : public Object
{
	using Super = Object;

public:
	Player();
	~Player() override;

	void Initialize() override;
	void Update() override;
	void Render(HDC InDC) override;

	// 직업군 별 처리가 필요한 경우 이런 식으로 함수를 정의함
	wstring GetPlayerName() const { return PlayerType == EPlayerType::CanonTank ? L"CanonTank" : L"MissileTank"; }

	void SetType(EPlayerType NewType) { PlayerType = NewType; }
	void SetId(int32 NewId) { Id = NewId; }
	void SetLocalPlayer(bool bInIsLocalPlayer) { bIsLocalPlayer = bInIsLocalPlayer; }

	int32 GetId() const { return Id; }
	EPlayerType GetPlayerType() const { return PlayerType; }
	bool IsLocallyControlled() const { return bIsLocalPlayer; }

private:
	EPlayerType PlayerType = EPlayerType::CanonTank;
	int32 Id = 0;  // 여러 Player들을 구분하기 위해 ID 설정
	uint8 bIsLocalPlayer = false;  // true이면 내 플레이어, 아니라면 상대방
	float FireAngle = 0.0f;
};

