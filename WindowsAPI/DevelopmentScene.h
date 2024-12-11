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
	void Update() override;  // 필요에 따라 FixedUpdate를 둬도 됨
	void Render(HDC DeviceContextHandle) override;

	void AddActor(AActor* NewActor);
	void RemoveActor(AActor* TargetActor);

private:
	// 추후 Base Scene 클래스로 이관
	vector<AActor*> Actors[static_cast<int32>(ELayerType::End)];  // 레이어 별로 액터를 두도록
	vector<UI*> UIs;  // UI는 UIManager를 둬도, Scene에 둬도 됨
};

