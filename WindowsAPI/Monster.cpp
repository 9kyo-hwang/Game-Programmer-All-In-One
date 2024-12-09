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

#if 1  // Inv Tri Example
	Position = Vector{ 400, 300 };
	LookAt = Vector{ 550, 70 };
	Direction = LookAt - Position;
	Direction.Normalize();
#endif
}

void Monster::Update()
{
#if 0  // Dot Product Example
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
#endif
}

void Monster::Render(HDC InDC)
{
	Utils::DrawCircle(InDC, Position, 100);

	// ���Ͱ� �ٶ󺸴� ����
	HPEN Pen = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN OldPen = (HPEN)::SelectObject(InDC, Pen);
	{
		Utils::DrawLine(InDC, Position, LookAt);
	}
	::SelectObject(InDC, OldPen);
	::DeleteObject(Pen);

	Vector MousePosition = InputManager::Get()->GetMousePosition();
	Vector MonsterToMouse = MousePosition - Position;
	MonsterToMouse.Normalize();
	Direction.Normalize();

	float DotRet = MonsterToMouse.Dot(Direction);
	float Radian = ::acos(DotRet);  // ��ũ �ڻ����� 0 ~ 180 ���� ���� ��Ÿ�� �� ����
	float Degree = Radian * 180 / (::atan(1) * 4);

	// �������� �ð�/�ݽð� ���� �Ǵ� �� �� ����
	if (Direction.Cross(MonsterToMouse) < 0)
	{
		Degree = 360 - Degree;
	}
	
	wstring Text = std::format(L"Degree: {0}", Degree);
	Utils::DrawTextW(InDC, { 20, 50 }, Text);

# if 0  // Dot, Cross Product Example
	Vector Mouse = InputManager::Get()->GetMousePosition();

	HPEN Pen = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN OldPen = (HPEN)::SelectObject(InDC, Pen);
	{
		// Utils::DrawLine(InDC, Start, End);  // �ι� ����
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

	float Cross1 = Point1ToPoint2.Cross(Point1ToMouse);  // 3D���ٸ� ���� ���Ͱ� ����
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
#endif
}
