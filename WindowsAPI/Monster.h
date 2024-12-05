#pragma once
#include "Object.h"

class Monster : public Object
{
	using Super = Object;

public:
	Monster();
	~Monster() override;

	void Initialize() override;
	void Update() override;
	void Render(HDC InDC) override;
};

