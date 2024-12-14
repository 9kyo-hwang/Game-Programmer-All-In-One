#include "pch.h"
#include "APlayer.h"

#include "CameraComponent.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "TimerManager.h"
#include "BoxCollider.h"

APlayer::APlayer()
{
	FB_MoveUp = ResourceManager::Get()->GetFlipbook(L"FB_MoveUp");
	FB_MoveDown = ResourceManager::Get()->GetFlipbook(L"FB_MoveDown");
	FB_MoveLeft = ResourceManager::Get()->GetFlipbook(L"FB_MoveLeft");
	FB_MoveRight = ResourceManager::Get()->GetFlipbook(L"FB_MoveRight");

	AddComponent(new CameraComponent());
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();

	SetFlipbook(FB_MoveRight);
}

void APlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO: ������ ���� 2D �÷����� �Է����� ����
	//if (InputManager::Get()->GetButton(EKeyCode::W))
	//{
	//	Position.Y -= 200 * DeltaTime;
	//	SetFlipbook(FB_MoveUp);
	//}
	//else if (InputManager::Get()->GetButton(EKeyCode::S))
	//{
	//	Position.Y += 200 * DeltaTime;
	//	SetFlipbook(FB_MoveDown);
	//}
	if (InputManager::Get()->GetButton(EKeyCode::A))
	{
		Position.X -= 200 * DeltaTime;
		SetFlipbook(FB_MoveLeft);
	}
	else if (InputManager::Get()->GetButton(EKeyCode::D))
	{
		Position.X += 200 * DeltaTime;
		SetFlipbook(FB_MoveRight);
	}
	else if (InputManager::Get()->GetButtonDown(EKeyCode::Space))
	{
		Jump();
	}

	OnTickGravity(DeltaTime);
}

void APlayer::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

void APlayer::OnComponentBeginOverlap(Collider* This, Collider* Other)
{
	if (BoxCollider* ThisBoxCollider = dynamic_cast<BoxCollider*>(This))
	{
		if (BoxCollider* OtherBoxCollider = dynamic_cast<BoxCollider*>(Other))
		{
			AdjustCollisionPos(ThisBoxCollider, OtherBoxCollider);
			bOnGround = true;
		}
	}
}

void APlayer::OnComponentEndOverlap(Collider* This, Collider* Other)
{
	Super::OnComponentEndOverlap(This, Other);
}

void APlayer::Jump()
{
}

void APlayer::OnTickGravity(float DeltaTime)
{
	if (DeltaTime > 0.1f)
	{
		return;
	}

	if (bOnGround)
	{
		return;
	}

	Speed.Y += Gravity * DeltaTime;
	Position.Y += Speed.Y * DeltaTime;
}

// ������ ���� �ݴ�� �ٽ� ���ļ� �ش� ĭ���� �������� ���ϵ��� �����ϴ� �Լ�
void APlayer::AdjustCollisionPos(BoxCollider* This, BoxCollider* Other)
{
	RECT ThisRect = This->GetRect();
	RECT OtherRect = Other->GetRect();

	RECT Intersect{};
	Vector2 NewPosition = GetPosition();
	if (::IntersectRect(&Intersect, &ThisRect, &OtherRect))
	{
		// Intersect �簢���� ���ΰ� ��� ���� ��������, ���ΰ� ��� ���� �������� ��
		int32 Width = Intersect.right - Intersect.left;
		int32 Height = Intersect.bottom - Intersect.top;

		if (Width > Height)
		{
			if (Intersect.top == OtherRect.top)
			{
				NewPosition.Y -= Height;
			}
			else
			{
				NewPosition.Y += Height;
			}
		}
		else
		{
			if (Intersect.left == OtherRect.left)
			{
				NewPosition.X -= Width;
			}
			else
			{
				NewPosition.X += Width;
			}
		}
	}

	SetPosition(NewPosition);
}
