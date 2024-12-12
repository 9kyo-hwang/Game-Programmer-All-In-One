#pragma once
#include "TimerManager.h"

class AActor;
class UI;

class Scene
{
public:
	Scene(){}
	virtual ~Scene(){}

	virtual void Initialize();
	virtual void Update(float DeltaTime = TimerManager::Get()->GetDeltaTime());
	virtual void Render(HDC DeviceContextHandle);

	void AddActor(AActor* NewActor);
	void RemoveActor(AActor* TargetActor);

protected:
	vector<AActor*> Actors[static_cast<int32>(ELayerType::End)];
	vector<UI*> UIs;
};