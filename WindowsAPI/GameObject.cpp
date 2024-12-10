#include "pch.h"
#include "GameObject.h"
#include "Component.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Start()
{
	for (Component* Item : Components)
	{
		Item->Start();
	}
}

void GameObject::Update()
{
	for (Component* Item : Components)
	{
		Item->Update();
	}
}

void GameObject::Render(HDC InDC)
{
	for (Component* Item : Components)
	{
		Item->Render(InDC);
	}
}

void GameObject::AddComponent(Component* NewComponent)
{
	Components.push_back(NewComponent);
}
