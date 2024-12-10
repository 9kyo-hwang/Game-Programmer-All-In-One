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
	void Render(HDC InDC) override;

	void FlipGameTurn();

private:
	uint8 Turn = 1;
	float ElapsedTime = 0.0f;
};

