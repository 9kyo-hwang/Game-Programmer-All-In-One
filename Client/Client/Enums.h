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

enum class EMovementDirection
{
	Up,
	Down,
	Left,
	Right,
	END
};

enum class EObjectStates
{
	Idle,
	Move,
	Attack,
	END
};

enum class EWeapons
{
	Sword,
	Bow,
	Staff,
};