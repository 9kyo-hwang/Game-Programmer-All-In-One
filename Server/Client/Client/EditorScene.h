#pragma once
#include "Scene.h"

class EditorScene : public Scene
{
	using Super = Scene;

public:
	EditorScene();
	~EditorScene() override;

	void Initialize() override;
	void Update(float DeltaTime) override;
	void Render(HDC InDC) override;

private:
	vector<pair<POINT, POINT>> Lines;

	bool bSetOrigin = true;
	POINT LastPosition{};
};

