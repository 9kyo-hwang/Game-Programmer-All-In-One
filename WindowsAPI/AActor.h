#pragma once

class Component;

class AActor
{
public:
	AActor();
	virtual ~AActor();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC InDC);

	Vector2 GetPosition() const { return Position; }
	void SetPosition(Vector2 NewPosition) { Position = NewPosition; }

	void AddComponent(Component* NewComponent);
	void RemoveComponent(Component* TargetComponent);

	ELayerType GetLayer() const { return Layer; }
	void SetLayer(ELayerType NewLayer) { Layer = NewLayer; }

protected:
	Vector2 Position{ 0, 0 };  // 씬에 배치 가능하므로 좌표 필요
	vector<Component*> Components;
	ELayerType Layer = ELayerType::Object;
};

