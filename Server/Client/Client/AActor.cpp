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

void AActor::Tick(float DeltaTime)
{
	for (Component* Item : Components)
	{
		Item->TickComponent();
	}
}

void AActor::Render(HDC DeviceContextHandle)
{
	for (Component* Item : Components)
	{
		Item->Render(DeviceContextHandle);
	}
}

Component* AActor::GetCollider() const
{
	for (Component* Item : Components)
	{
		if (Collider* Col = dynamic_cast<Collider*>(Item))
		{
			return Col;
		}
	}

	return nullptr;
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

