#include "pch.h"
#include "Object.h"
#include "ObjectManager.h"

ObjectManager::~ObjectManager()
{
	Clear();
}

void ObjectManager::Add(Object* NewObject)
{
	if (!NewObject)
	{
		return;
	}

	if (ranges::find(Objects, NewObject) != Objects.end())
	{
		return;
	}

	Objects.push_back(NewObject);
}

void ObjectManager::Remove(Object* TargetObject)
{
	if (!TargetObject)
	{
		return;
	}

	Objects.erase(ranges::remove(Objects, TargetObject).begin(), Objects.end());

	// TODO: ±¦ÂúÀ»±î?
	delete TargetObject;
}

void ObjectManager::Clear()
{
	ranges::for_each(Objects, [=](Object* Element) { delete Element; });
	Objects.clear();
}
