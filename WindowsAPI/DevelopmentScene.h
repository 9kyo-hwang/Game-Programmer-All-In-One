#pragma once
#include "Scene.h"

class AActor;

class DevelopmentScene final : public Scene
{
	using Super = Scene;

public:
	DevelopmentScene();
	~DevelopmentScene() override;

	void Initialize() override;
	void Update() override;  // �ʿ信 ���� FixedUpdate�� �ֵ� ��
	void Render(HDC InDC) override;

private:
	vector<AActor*> Actors;
};

