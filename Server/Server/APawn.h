#pragma once
#include "AActor.h"

class APawn : public AActor
{
	using Super = AActor;

public:
	APawn() {}
	~APawn() override {}

	void Initialize() override {}
	void Update() override {}
};

