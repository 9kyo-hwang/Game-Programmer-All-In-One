#pragma once
#include "APawn.h"
class APlayer : public APawn
{
	using Super = APawn;

public:
	APlayer();
	~APlayer() override;

	inline static Protocol::EObjectType Type = Protocol::Player;

public:
	// 서버로부터 배치된 플레이어끼리 식별하기 위해 세션 정보를 들고 있음
	// 언리얼의 컨트롤러 역할인듯...?
	SessionRef Session;
};

