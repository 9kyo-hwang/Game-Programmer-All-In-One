#pragma once

enum class EObjectType : uint8
{
	None,
	Player,
	Monster,
	Projectile,
};

enum class EDirection : uint8
{
	Left,
	Right,
};

class Object
{
public:
	struct Stats
	{
		int32 Hp = 0;
		int32 MaxHp = 0;
		float Speed = 0.0f;
	};

	Object(EObjectType InType);
	virtual ~Object();

	virtual void Initialize() abstract;
	virtual void Update() abstract;
	virtual void Render(HDC InDC) abstract;

	EObjectType GetObjectType() const { return Type; }
	Vector GetPosition() const { return Position; }
	void SetPosition(Vector NewPosition) { Position = NewPosition; }
	float GetRadius() const { return Radius; }
	void SetRadius(float NewRadius) { Radius = NewRadius; }

protected:
	EObjectType Type = EObjectType::None;
	EDirection Direction = EDirection::Left;
	Vector Position{};
	Stats Stat{};
	float Radius = 50.f;  // 일종의 충돌 범위
};

