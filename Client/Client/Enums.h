#pragma once

enum class ERenderLayer : uint8
{
	Background,  // 백그라운드가 가장 먼저 그려져야 함.
	Object,
	UI,
	End
};

enum class ECollideLayer : uint8
{
	Object,
	Ground,
	Wall,

};

enum class EMovementDirection : uint8
{
	Up,
	Down,
	Left,
	Right,
	END
};