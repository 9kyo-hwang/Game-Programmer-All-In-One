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

private:
	Vector Start{ 300, 100 };
	Vector End{ 600, 250 };

	Vector Point1{ 400, 150 };
	Vector Point2{ 600, 450 };
	Vector Point3{ 200,450 };
};

