#include "pch.h"
#include "CameraComponent.h"

#include "AActor.h"
#include "SceneManager.h"

CameraComponent::CameraComponent()
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::BeginPlay()
{
}

void CameraComponent::TickComponent()
{
	if (Owner)
	{
		Vector2 OwnerPosition = Owner->GetCurrentPosition();
		OwnerPosition.X = ::clamp(OwnerPosition.X, 400.0f, 3024.0f - 400.0f);
		OwnerPosition.Y = ::clamp(OwnerPosition.Y, 300.0f, 2064.0f - 300.0f);
		SceneManager::Get()->SetCameraPosition(OwnerPosition);
	}
}

void CameraComponent::Render(HDC InDC)
{
}
