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
	TWeakPtr<class APlayer> Target;  // Client와 달리 Server에서는 WeakPtr로 들고 있자
	uint64 WaitUntil = 0;
};

