#pragma once
#include "TimerManager.h"

class Component;
class Collider;

class AActor
{
public:
	AActor();
	virtual ~AActor();

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime = TimerManager::Get()->GetDeltaTime());
	virtual void Render(HDC DeviceContextHandle);

	Vector2 GetPosition() const { return Position; }
	void SetPosition(Vector2 NewPosition) { Position = NewPosition; }

	void AddComponent(Component* NewComponent);
	void RemoveComponent(Component* TargetComponent);

	ELayerType GetLayer() const { return Layer; }
	void SetLayer(ELayerType NewLayer) { Layer = NewLayer; }

	virtual void OnComponentBeginOverlap(Collider* This, Collider* Other);
	virtual void OnComponentEndOverlap(Collider* This, Collider* Other);

protected:
	Vector2 Position{ 0, 0 };  // 씬에 배치 가능하므로 좌표 필요
	vector<Component*> Components;
	ELayerType Layer = ELayerType::Object;
};

