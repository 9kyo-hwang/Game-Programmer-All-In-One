#pragma once

class Scene
{
public:
	Scene(){}
	virtual ~Scene(){}

	virtual void Initialize() abstract;
	virtual void Update() abstract;
	virtual void Render(HDC InDC) abstract;
};