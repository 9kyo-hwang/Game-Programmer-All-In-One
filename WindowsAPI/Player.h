#pragma once
#include "Object.h"

class Player : public Object
{
	using Super = Object;

public:
	Player();
	virtual ~Player() override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC InDC) override;
};

