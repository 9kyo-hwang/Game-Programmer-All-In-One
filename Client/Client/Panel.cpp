#include "pch.h"
#include "Panel.h"

Panel::Panel()
{
}

Panel::~Panel()
{
	for (UI* Child : Children)
	{
		SAFE_DELETE(Child);
	}

	Children.clear();
}

void Panel::BeginPlay()
{
	Super::BeginPlay();

	for (UI* Child : Children)
	{
		Child->BeginPlay();
	}
}

void Panel::Tick()
{
	Super::Tick();

	for (UI* Child : Children)
	{
		Child->Tick();
	}
}

void Panel::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);

	for (UI* Child : Children)
	{
		Child->Render(DeviceContextHandle);
	}
}

void Panel::AddChild(UI* Child)
{
	if (Child)
	{
		Children.push_back(Child);
	}
}

bool Panel::RemoveChild(UI* Child)
{
	auto It = ranges::find(Children, Child);
	if (It == Children.end())
	{
		return false;
	}

	// TODO: Erase?
	Children.erase(It);
	return true;
}
