#pragma once

enum class ERenderLayer : uint8
{
	Background,  // ��׶��尡 ���� ���� �׷����� ��.
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