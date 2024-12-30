#pragma once
#include "APawn.h"
class AMonster : public APawn
{
	using Super = APawn;

public:
	AMonster();
	~AMonster() override;

	inline static Protocol::EObjectType Type = Protocol::Monster;

	virtual void Tick();
};

