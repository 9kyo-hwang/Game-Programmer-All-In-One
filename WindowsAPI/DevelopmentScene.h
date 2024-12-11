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

	void AddActor(AActor* NewActor);
	void RemoveActor(AActor* TargetActor);

private:
	vector<AActor*> Actors[static_cast<int32>(ELayerType::End)];  // ���̾� ���� ���͸� �ε���
};

