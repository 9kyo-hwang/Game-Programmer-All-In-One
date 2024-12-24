#pragma once

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

struct Vector2
{
	Vector2() {}
	Vector2(float InX, float InY)
		: X(InX)
		, Y(InY)
	{}
	Vector2(POINT Point)
		: X(static_cast<float>(Point.x))
		, Y(static_cast<float>(Point.y))
	{}

	Vector2 operator+(const Vector2& Other) const
	{
		return { X + Other.X, Y + Other.Y };
	}


	Vector2 operator-(const Vector2& Other) const
	{
		return { X - Other.X, Y - Other.Y };
	}

	Vector2 operator*(const float Value) const
	{
		return { X * Value, Y * Value };
	}

	void operator+=(const Vector2& Other)
	{
		X += Other.X;
		Y += Other.Y;
	}

	void operator-=(const Vector2& Other)
	{
		X -= Other.X;
		Y -= Other.Y;
	}

	void operator*=(const float Value)
	{
		X *= Value;
		Y *= Value;
	}

	float GetMagnitudeSquared() const
	{
		return X * X + Y * Y;
	}

	float GetMagnitude() const
	{
		// 루트 연산은 비쌈
		return ::sqrt(GetMagnitudeSquared());
	}

	void Normalize()
	{
		const float Magnitude = GetMagnitude();
		if (Magnitude < std::numeric_limits<float>::epsilon())
		{
			return;
		}

		X /= Magnitude;
		Y /= Magnitude;
	}

	float Dot(const Vector2& Other) const
	{
		return X * Other.X + Y * Other.Y;
	}

	float Cross(const Vector2& Other) const
	{
		return X * Other.Y - Y * Other.X;  // 양수, 음수냐에 따라 방향 달라짐
	}

	float X = 0.0f;
	float Y = 0.0f;
};

struct Vector2Int
{
	Vector2Int() {}
	Vector2Int(int32 InX, int32 InY)
		: X(InX)
		, Y(InY)
	{}
	Vector2Int(POINT Point)
		: X(static_cast<int32>(Point.x))
		, Y(static_cast<int32>(Point.y))
	{}

	Vector2Int operator+(const Vector2Int& Other) const
	{
		return { X + Other.X , Y + Other.Y };
	}

	Vector2Int operator-(const Vector2Int& Other) const
	{
		return { X - Other.X, Y - Other.Y };
	}

	Vector2Int operator*(const int32 Value) const
	{
		return { X * Value, Y * Value };
	}

	void operator+=(const Vector2Int& Other)
	{
		X += Other.X;
		Y += Other.Y;
	}

	void operator-=(const Vector2Int& Other)
	{
		X -= Other.X;
		Y -= Other.Y;
	}

	int32 GetMagnitudeSquared() const
	{
		return X * X + Y * Y;
	}

	float GetMagnitude() const
	{
		return ::sqrt(GetMagnitudeSquared());
	}

	int32 Dot(const Vector2Int& Other) const
	{
		return X * Other.X + Y * Other.Y;
	}

	int32 Cross(const Vector2Int& Other) const
	{
		return X * Other.Y - Y * Other.X;  // 양수, 음수냐에 따라 방향 달라짐
	}

	int32 X = 0;
	int32 Y = 0;
};

struct UAttributeSet
{
	// 각각의 요소가 AttributeData
	int32 Hp = 0;
	int32 MaxHp = 0;
	int32 Attack = 0;
	int32 Defence = 0;
	float Speed = 0.f;
};