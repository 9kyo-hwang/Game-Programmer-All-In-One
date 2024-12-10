#pragma once
#include "Scene.h"

class MenuScene final : public Scene
{
	using Super = Scene;

public:
	MenuScene();
	~MenuScene() override;

	void Initialize() override;
	void Update() override;
	void Render(HDC InDC) override;
};

