#pragma once
#include "Scene.h"

class AActor;
class UI;

class DevelopmentScene final : public Scene
{
	using Super = Scene;

public:
	DevelopmentScene();
	~DevelopmentScene() override;

	void Initialize() override;
	void Update() override;  // �ʿ信 ���� FixedUpdate�� �ֵ� ��
	void Render(HDC DeviceContextHandle) override;

	void AddActor(AActor* NewActor);
	void RemoveActor(AActor* TargetActor);

private:
	// ���� Base Scene Ŭ������ �̰�
	vector<AActor*> Actors[static_cast<int32>(ELayerType::End)];  // ���̾� ���� ���͸� �ε���
	vector<UI*> UIs;  // UI�� UIManager�� �ֵ�, Scene�� �ֵ� ��
};

