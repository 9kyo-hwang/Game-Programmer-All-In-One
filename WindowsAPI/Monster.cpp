#include "pch.h"
#include "Monster.h"

#include "InputManager.h"

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
	Vector StartToEnd = End - Start;  // Start -> End
	float Magnitude = StartToEnd.GetMagnitude();
	StartToEnd.Normalize();

	Vector Mouse = InputManager::Get()->GetMousePosition();
	Vector StartToMouse = Mouse - Start;  // Start -> MousePosition

	// ��) � ������ ��ܾ� �� ��, ������ �̿��� ������ �˵��� ������ �� ����
	// ���⿡ Projection�� �̿��� ���� ��ġ������ ������ �� �� ����
	float DotRet = StartToEnd.Dot(StartToMouse);
	if (0 <= DotRet && DotRet <= Magnitude)
	{
		Position = Start + StartToEnd * DotRet;
	}
}

void Monster::Render(HDC InDC)
{
	Utils::DrawCircle(InDC, Position, 50);

	HPEN Pen = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN OldPen = (HPEN)::SelectObject(InDC, Pen);
	{
		// �ι� ����
		Utils::DrawLine(InDC, Start, End);
	}
	::SelectObject(InDC, OldPen);
	::DeleteObject(Pen);
}
