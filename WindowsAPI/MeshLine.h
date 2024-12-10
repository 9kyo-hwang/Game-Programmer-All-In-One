#pragma once
#include "Resources.h"

class MeshLine : public Resources
{
public:
	void Save(wstring Path);
	void Load(wstring Path);
	void Render(HDC InDC, Vector Position, float RatioX = 1.0f, float RatioY = 1.0f) const;

protected:
	vector<pair<POINT, POINT>> Lines;
	int32 Width = 0;
	int32 Height = 0;
};

