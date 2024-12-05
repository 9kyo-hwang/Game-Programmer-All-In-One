#pragma once
#include "Singleton.h"

class MeshLine;

class ResourceManager : public Singleton<ResourceManager>
{
public:
	~ResourceManager();

	void Initialize();
	void Clear();

	const MeshLine* GetMeshLine(wstring Name);

private:
	unordered_map<wstring, MeshLine*> MeshLines;
};

