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