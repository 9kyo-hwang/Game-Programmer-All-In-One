#pragma once

class Component;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Start();
	virtual void Update();
	virtual void Render(HDC InDC);

	void AddComponent(Component* NewComponent);
	void SetPosition(Vector2Int NewPosition) { Position = NewPosition; }

protected:
	Vector2Int Position{};  // Unity�� Transform ������Ʈ ����
	vector<Component*> Components;
};
