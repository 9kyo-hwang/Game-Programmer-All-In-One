#pragma once

enum class ELayerType : uint8
{
	Background,  // 백그라운드가 가장 먼저 그려져야 함.
	Object,
	UI,
	End
};