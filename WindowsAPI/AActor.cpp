#include "pch.h"
#include "AActor.h"
#include "Component.h"
#include "Collider.h"

AActor::AActor()
{
}

AActor::~AActor()
{
}

void AActor::BeginPlay()
{
	for (Component* Item : Components)
	{
		Item->BeginPlay();
	}
}

void AActor::Tick()
{
	for (Component* Item : Components)
	{
		Item->TickComponent();
	}
}

void AActor::Render(HDC InDC)
{
	for (Component* Item : Components)
	{
		Item->Render(InDC);
	}
}

void AActor::AddComponent(Component* NewComponent)
{
	if (NewComponent)
	{
		NewComponent->SetOwner(this);
		Components.push_back(NewComponent);
	}
}

void AActor::RemoveComponent(Component* TargetComponent)
{
	if (TargetComponent)
	{
		std::erase(Components, TargetComponent);
	}
}

void AActor::OnComponentBeginOverlap(Collider* This, Collider* Other)
{
	
}

void AActor::OnComponentEndOverlap(Collider* This, Collider* Other)
{
	
}

