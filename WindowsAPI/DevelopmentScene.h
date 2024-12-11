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
	void Update() override;  // 필요에 따라 FixedUpdate를 둬도 됨
	void Render(HDC InDC) override;

	void AddActor(AActor* NewActor);
	void RemoveActor(AActor* TargetActor);

private:
	vector<AActor*> Actors[static_cast<int32>(ELayerType::End)];  // 레이어 별로 액터를 두도록
};

