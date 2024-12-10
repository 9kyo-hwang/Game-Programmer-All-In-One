#pragma once
#include "Component.h"

class CharacterMovementComponent : public Component
{
public:
	void Start() override;
	void Update() override;
	void Render(HDC InDC) override;

};

