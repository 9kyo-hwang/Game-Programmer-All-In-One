#pragma once
#include "Component.h"

class CameraComponent : public Component
{
	using Super = Component;

public:
	CameraComponent();
	~CameraComponent() override;

	void BeginPlay() override;
	void TickComponent() override;
	void Render(HDC InDC) override;
};

