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
	{
		MeshLine* Mesh = new MeshLine();
		Mesh->Load(L"UI.txt");
		MeshLines[L"UI"] = Mesh;
	}
	{
		MeshLine* Mesh = new MeshLine();
		Mesh->Load(L"Menu.txt");
		MeshLines[L"Menu"] = Mesh;
	}
	{
		MeshLine* Mesh = new MeshLine();
		Mesh->Load(L"MissileTank.txt");
		MeshLines[L"MissileTank"] = Mesh;
	}
	{
		MeshLine* Mesh = new MeshLine();
		Mesh->Load(L"CanonTank.txt");
		MeshLines[L"CanonTank"] = Mesh;
	}
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
