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
	void Render(HDC InHandleDeviceContext) override;

public:
	// TEST
	Vector2 PlayerPosition{300, 300};
	float Speed = 1000.0f;
};

