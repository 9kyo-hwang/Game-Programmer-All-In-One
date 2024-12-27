#include "pch.h"
#include "Scene.h"
#include "AActor.h"
#include "APawn.h"
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
	// Effect같은 것들은 수시로 생성되고 지워지기 때문에 참조로 돌면 큰일남
	for (const vector<AActor*> ActorsOnLayer : Actors)
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
	// Y축이 작은 쪽을 먼저 그리자
	ranges::sort(Actors[static_cast<int32>(ERenderLayer::Object)], 
		[=](const AActor* Lhs, const AActor* Rhs)
		{
			return Lhs->GetCurrentPosition().Y < Rhs->GetCurrentPosition().Y;
		});

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

void Scene::SpawnActor(AActor* NewActor)
{
	if (NewActor)
	{
		Actors[static_cast<int32>(NewActor->GetLayer())].push_back(NewActor);
	}
}

void Scene::DestroyActor(AActor* TargetActor)
{
	if (TargetActor)
	{
		std::erase(Actors[static_cast<int32>(TargetActor->GetLayer())], TargetActor);
	}
}

APawn* Scene::GetPawnAt(Vector2Int Cell) const
{
	for (AActor* Actor : Actors[static_cast<int32>(ERenderLayer::Object)])
	{
		if (APawn* Pawn = dynamic_cast<APawn*>(Actor))
		{
			if (Pawn->GetCellPosition() == Cell)
			{
				return Pawn;
			}
		}
	}

	return nullptr;
}
