#pragma once
#include "Resources.h"

class MeshLine : public Resources
{
public:
	void Save(wstring Path);
	void Load(wstring Path);
	void Render(HDC InDC, Vector2 Position) const;

protected:
	vector<pair<POINT, POINT>> Lines;
};

