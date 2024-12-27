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

	virtual void SpawnActor(AActor* NewActor);
	virtual void DestroyActor(AActor* TargetActor);

	class APawn* GetPawnAt(Vector2Int Cell) const;

protected:
	vector<AActor*> Actors[static_cast<int32>(ERenderLayer::End)];
	vector<UI*> UIs;
};