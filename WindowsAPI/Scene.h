#pragma once

class Scene
{
public:
	Scene(){}
	virtual ~Scene(){}

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render(HDC InDC) = 0;
};