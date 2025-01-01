#pragma once

enum class ERenderLayer
{
	Background,  // ��׶��尡 ���� ���� �׷����� ��.
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