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

	// ������ �� ó���� �ʿ��� ��� �̷� ������ �Լ��� ������
	wstring GetPlayerName() const { return PlayerType == EPlayerType::CanonTank ? L"CanonTank" : L"MissileTank"; }

	void SetType(EPlayerType NewType) { PlayerType = NewType; }
	void SetId(int32 NewId) { Id = NewId; }
	void SetLocalPlayer(bool bInIsLocalPlayer) { bIsLocalPlayer = bInIsLocalPlayer; }

	int32 GetId() const { return Id; }
	EPlayerType GetPlayerType() const { return PlayerType; }
	bool IsLocallyControlled() const { return bIsLocalPlayer; }

private:
	EPlayerType PlayerType = EPlayerType::CanonTank;
	int32 Id = 0;  // ���� Player���� �����ϱ� ���� ID ����
	uint8 bIsLocalPlayer = false;  // true�̸� �� �÷��̾�, �ƴ϶�� ����
	float FireAngle = 0.0f;
};

