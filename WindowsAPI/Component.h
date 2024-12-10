#pragma once

class Component
{
public:
	virtual void Start();
	virtual void Update();
	virtual void Render(HDC InDC);

protected:
	// Owner
	class GameObject* Parent = nullptr;
};

