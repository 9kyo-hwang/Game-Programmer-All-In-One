#include "pch.h"
#include "Scene.h"
#include "AActor.h"
#include "CollisionManager.h"
#include "UI.h"

void Scene::Initialize()
{
	for (const vector<AActor*>& ActorsPerLayer : Actors)
	{
		for (AActor* Actor : ActorsPerLayer)
		{
			Actor->BeginPlay();
		}
	}

	for (UI* Item : UIs)
	{
		Item->BeginPlay();
	}
}

void Scene::Update(float DeltaTime)
{
	for (const vector<AActor*>& ActorsOnLayer : Actors)
	{
		for (AActor* Actor : ActorsOnLayer)
		{
			Actor->Tick();
		}
	}

	for (UI* Item : UIs)
	{
		Item->Tick();
	}

	CollisionManager::Get()->Update();
}

void Scene::Render(HDC DeviceContextHandle)
{
	for (const vector<AActor*>& ActorsOnLayer : Actors)
	{
		for (AActor* Actor : ActorsOnLayer)
		{
			Actor->Render(DeviceContextHandle);
		}
	}

	for (UI* Item : UIs)
	{
		Item->Render(DeviceContextHandle);
	}

	CollisionManager::Get()->Update();
}

void Scene::AddActor(AActor* NewActor)
{
	if (NewActor)
	{
		Actors[static_cast<int32>(NewActor->GetLayer())].push_back(NewActor);
	}
}

void Scene::RemoveActor(AActor* TargetActor)
{
	if (TargetActor)
	{
		std::erase(Actors[static_cast<int32>(TargetActor->GetLayer())], TargetActor);
	}
}