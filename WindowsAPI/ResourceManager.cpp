#include "pch.h"
#include "ResourceManager.h"

#include <ranges>

#include "MeshLine.h"

ResourceManager::~ResourceManager()
{
	Clear();
}

void ResourceManager::Initialize()
{
	MeshLine* Mesh = new MeshLine();
	Mesh->Load(L"Player.txt");

	MeshLines[L"Player"] = Mesh;
}

void ResourceManager::Clear()
{
	for (auto Mesh : MeshLines | views::values)
	{
		SAFE_DELETE(Mesh);
	}

	MeshLines.clear();
}

const MeshLine* ResourceManager::GetMeshLine(wstring Name)
{
	if (MeshLines.contains(Name))
	{
		return MeshLines[Name];
	}

	return nullptr;
}
