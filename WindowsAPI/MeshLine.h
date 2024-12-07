#pragma once
#include "Resources.h"

class MeshLine : public Resources
{
public:
	void Save(wstring Path);
	void Load(wstring Path);
	void Render(HDC InDC, Vector Position) const;

protected:
	vector<pair<POINT, POINT>> Lines;
};

