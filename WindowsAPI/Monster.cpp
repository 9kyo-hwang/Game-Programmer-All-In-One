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
#if 0  // Dot Product Example
	Vector StartToEnd = End - Start;  // Start -> End
	float Magnitude = StartToEnd.GetMagnitude();
	StartToEnd.Normalize();

	Vector Mouse = InputManager::Get()->GetMousePosition();
	Vector StartToMouse = Mouse - Start;  // Start -> MousePosition

	// 예) 어떤 물건을 당겨야 할 때, 내적을 이용해 일직선 궤도를 유지할 수 있음
	// 여기에 Projection을 이용해 수직 위치까지만 오도록 할 수 있음
	float DotRet = StartToEnd.Dot(StartToMouse);
	if (0 <= DotRet && DotRet <= Magnitude)
	{
		Position = Start + StartToEnd * DotRet;
	}
#endif
}

void Monster::Render(HDC InDC)
{
	// Utils::DrawCircle(InDC, Position, 50);

	Vector Mouse = InputManager::Get()->GetMousePosition();

	HPEN Pen = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN OldPen = (HPEN)::SelectObject(InDC, Pen);
	{
		// Utils::DrawLine(InDC, Start, End);  // 로밍 라인
		Utils::DrawLine(InDC, Point1, Point2);
		Utils::DrawLine(InDC, Point2, Point3);
		Utils::DrawLine(InDC, Point3, Point1);
	}
	::SelectObject(InDC, OldPen);
	::DeleteObject(Pen);

	Utils::DrawLine(InDC, Point1, Mouse);
	Vector Point1ToPoint2 = Point2 - Point1; Point1ToPoint2.Normalize();
	Vector Point1ToMouse = Mouse - Point1;   Point1ToMouse.Normalize();
	Vector Point1ToPoint3 = Point3 - Point1; Point1ToPoint3.Normalize();

	float Cross1 = Point1ToPoint2.Cross(Point1ToMouse);  // 3D였다면 방향 벡터가 나옴
	float Cross2 = Point1ToMouse.Cross(Point1ToPoint3);

	if (Cross1 >= 0 && Cross2 >= 0)
	{
		// TODO: ...
	}
	else
	{
		
	}

	wstring str = std::format(L"Cross 1: {0}, Cross 2: {1}", Cross1, Cross2);
	Utils::DrawTextW(InDC, { 20, 50 }, str);
}
