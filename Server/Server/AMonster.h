#pragma once
#include "APawn.h"

class AMonster : public APawn
{
	using Super = APawn;

public:
	AMonster();
	~AMonster() override;

	inline static Protocol::EObjectType Type = Protocol::Monster;

	void Initialize() override;
	void Update() override;

protected:
	virtual void OnUpdateIdle();
	virtual void OnUpdateMove();
	virtual void OnUpdateAttack();

protected:
	TWeakPtr<class APlayer> Target;  // Client�� �޸� Server������ WeakPtr�� ��� ����
	uint64 WaitUntil = 0;
};

