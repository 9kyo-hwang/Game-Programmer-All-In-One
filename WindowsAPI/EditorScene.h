#pragma once
#include "Scene.h"

class EditorScene : public Scene
{
	using Super = Scene;

public:
	EditorScene();
	virtual ~EditorScene() override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HDC InDC) override;

private:
	vector<pair<POINT, POINT>> Lines;

	bool bSetOrigin = true;
	POINT LastPosition{};
};

