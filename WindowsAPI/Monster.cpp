#include "pch.h"
#include "Monster.h"

Monster::Monster()
	: Super(EObjectType::Monster)
{
}

Monster::~Monster()
{
}

void Monster::Initialize()
{
	Stat.Hp = 100;
	Stat.MaxHp = 100;
	Stat.Speed = 10;
}

void Monster::Update()
{
}

void Monster::Render(HDC InDC)
{
	Utils::DrawRectangle(InDC, Position, 50, 50);
}
