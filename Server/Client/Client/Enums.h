#pragma once

enum class ERenderLayer
{
	Background,  // 백그라운드가 가장 먼저 그려져야 함.
	Object,
	Effect,
	UI,
	End
};

enum class ECollideLayer
{
	Object,
	Ground,
	Wall,
};

using EMovementDirection = Protocol::EDirection;
using EObjectStates = Protocol::EObjectState;

enum class EWeapons
{
	Sword,
	Bow,
	Staff,
};