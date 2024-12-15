#include "pch.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "AActor.h"
#include "SceneManager.h"

BoxCollider::BoxCollider()
	: Super(EColliderType::Box)
{
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::BeginPlay()
{
	Super::BeginPlay();
}

void BoxCollider::TickComponent()
{
	Super::TickComponent();
}

void BoxCollider::Render(HDC DeviceContextHandle)
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
		Utils::DrawRectangle(DeviceContextHandle, OwnerPosition, Size.X, Size.Y);
		::SelectObject(DeviceContextHandle, OldBrush);
		::DeleteObject(Brush);
	}
}

bool BoxCollider::IsCollision(Collider* Other)
{
	if (!Super::IsCollision(Other))
	{
		return false;
	}

	// 콜라이더 타입마다 다르게 연산해줘야...
	switch (Other->GetColliderType())
	{
	case EColliderType::Box:
		return OnCollision(static_cast<BoxCollider*>(Other), this);
	case EColliderType::Sphere:
		return OnCollision(static_cast<SphereCollider*>(Other), this);
	}

	return false;
}

RECT BoxCollider::GetRect() const
{
	Vector2 Position = GetOwner()->GetPosition();
	return
	{
		static_cast<int32>(Position.X - Size.X / 2),
		static_cast<int32>(Position.Y - Size.Y / 2),
		static_cast<int32>(Position.X + Size.X / 2),
		static_cast<int32>(Position.Y + Size.Y / 2),
	};
}
