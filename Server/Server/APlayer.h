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
	// �����κ��� ��ġ�� �÷��̾�� �ĺ��ϱ� ���� ���� ������ ��� ����
	// �𸮾��� ��Ʈ�ѷ� �����ε�...?
	SessionRef Session;
};

