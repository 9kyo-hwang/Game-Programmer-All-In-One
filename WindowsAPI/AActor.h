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
	Vector2 Position{ 0, 0 };  // ���� ��ġ �����ϹǷ� ��ǥ �ʿ�
	vector<Component*> Components;
	ELayerType Layer = ELayerType::Object;
};

