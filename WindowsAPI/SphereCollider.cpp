#include "pch.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "AActor.h"
#include "SceneManager.h"

SphereCollider::SphereCollider()
	: Super(EColliderType::Sphere)
{
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::BeginPlay()
{
	Super::BeginPlay();
}

void SphereCollider::TickComponent()
{
	Super::TickComponent();
}

void SphereCollider::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);

	if (bShowDebug)
	{
		Vector2 Camera = SceneManager::Get()->GetCameraPosition();
		Vector2 OwnerPosition = GetOwner()->GetPosition();
		OwnerPosition.X -= (Camera.X - GWinSizeX / 2);
		OwnerPosition.Y -= (Camera.Y - GWinSizeY / 2);

		HBRUSH Brush = static_cast<HBRUSH>(::GetStockObject(NULL_BRUSH));
		HBRUSH OldBrush = static_cast<HBRUSH>(::SelectObject(DeviceContextHandle, Brush));
		Utils::DrawCircle(DeviceContextHandle, OwnerPosition, Radius);
		::SelectObject(DeviceContextHandle, OldBrush);
		::DeleteObject(Brush);
	}
}

bool SphereCollider::IsCollision(Collider* Other)
{
	if (!Super::IsCollision(Other))
	{
		return false;
	}

	// 콜라이더 타입마다 다르게 연산해줘야...
	switch (Other->GetColliderType())
	{
	case EColliderType::Box:
		return OnCollision(this, static_cast<BoxCollider*>(Other));
	case EColliderType::Sphere:
		return OnCollision(this, static_cast<SphereCollider*>(Other));
	}

	return false;
}
