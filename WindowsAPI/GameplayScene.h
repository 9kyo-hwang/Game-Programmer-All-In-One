#pragma once
#include "Scene.h"

class GameplayScene final : public Scene
{
	using Super = Scene;

public:
	GameplayScene();
	~GameplayScene() override;

	void Initialize() override;
	void Update() override;
	void Render(HDC InHandleDeviceContext) override;
};

