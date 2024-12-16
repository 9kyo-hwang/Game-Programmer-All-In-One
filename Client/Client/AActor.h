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

	Vector2 GetCurrentPosition() const { return CurrentPosition; }
	void SetCurrentPosition(Vector2 NewPosition) { CurrentPosition = NewPosition; }

	Component* GetCollider() const;
	void AddComponent(Component* NewComponent);
	void RemoveComponent(Component* TargetComponent);

	ERenderLayer GetLayer() const { return Layer; }
	void SetLayer(ERenderLayer NewLayer) { Layer = NewLayer; }

	// OnCollisionEnter2D, OnCollisionExit2D
	virtual void OnComponentBeginOverlap(Collider* This, Collider* Other);
	virtual void OnComponentEndOverlap(Collider* This, Collider* Other);

protected:
	Vector2 CurrentPosition{ 0, 0 };
	Vector2 DestinationPosition{ 0, 0 };
	vector<Component*> Components;
	ERenderLayer Layer = ERenderLayer::Object;
};

