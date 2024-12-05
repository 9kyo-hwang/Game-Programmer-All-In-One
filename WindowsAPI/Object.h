#pragma once

enum class EObjectType : uint8
{
	None,
	Player,
	Monster,
	Projectile,
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

	EObjectType GetType() const { return Type; }
	Vector2 GetPosition() const { return Position; }
	void SetPosition(Vector2 NewPosition) { Position = NewPosition; }

protected:
	EObjectType Type = EObjectType::None;
	Vector2 Position{};
	Stats Stat{};
};

