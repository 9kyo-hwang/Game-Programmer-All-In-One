#pragma once

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

struct Vector
{
	Vector() {}
	Vector(float InX, float InY)
		: X(InX)
		, Y(InY)
	{}
	Vector(POINT Point)
		: X(static_cast<float>(Point.x))
		, Y(static_cast<float>(Point.y))
	{}

	Vector operator+(const Vector& Other) const
	{
		Vector Ret;
		Ret.X = X + Other.X;
		Ret.Y = Y + Other.Y;

		return Ret;
	}

	Vector operator-(const Vector& Other) const
	{
		Vector Ret;
		Ret.X = X - Other.X;
		Ret.Y = Y - Other.Y;

		return Ret;
	}

	Vector operator*(const float Value) const
	{
		Vector Ret;
		Ret.X = X * Value;
		Ret.Y = Y * Value;

		return Ret;
	}

	void operator+=(const Vector& Other)
	{
		X += Other.X;
		Y += Other.Y;
	}

	void operator-=(const Vector& Other)
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

	float Dot(const Vector& Other) const
	{
		return X * Other.X + Y * Other.Y;
	}

	float Cross(const Vector& Other) const
	{
		return X * Other.Y - Y * Other.X;  // 양수, 음수냐에 따라 방향 달라짐
	}

	float X = 0.0f;
	float Y = 0.0f;
};
