#pragma once

#include "CorePCH.h"

#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib)
#endif

using SessionRef = TSharedPtr<class GameSession>;

#include "ServerPacketHandler.h"

using EMovementDirection = Protocol::EDirection;
using EObjectStates = Protocol::EObjectState;

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

	bool operator==(const Vector2Int& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	bool operator!=(const Vector2Int& Other) const
	{
		return !(*this == Other);
	}

	bool operator<(const Vector2Int& Other) const
	{
		return X != Other.X ? X < Other.X : Y < Other.Y;
	}

	bool operator>(const Vector2Int& Other) const
	{
		return X != Other.X ? X > Other.X : Y > Other.Y;
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