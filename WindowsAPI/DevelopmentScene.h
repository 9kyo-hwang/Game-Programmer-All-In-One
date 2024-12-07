#pragma once
#include "Scene.h"

class DevelopmentScene final : public Scene
{
	using Super = Scene;

public:
	DevelopmentScene();
	~DevelopmentScene() override;

	void Initialize() override;
	void Update() override;
	void Render(HDC InDC) override;

public:
	// TEST
	Vector PlayerPosition{300, 300};
	float Speed = 1000.0f;
};

