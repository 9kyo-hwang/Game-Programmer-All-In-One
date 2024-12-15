#include "pch.h"
#include "APlayer.h"

#include "CameraComponent.h"
#include "InputManager.h"
#include "ResourceManager.h"
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

	switch (CurrentState)
	{
	case EPlayerState::MoveOnGround:
		OnMove();
		OnInput(DeltaTime);
		break;
	case EPlayerState::Fall:
		OnFall();
		OnInput(DeltaTime);
		break;
	}

	OnTickGravity(DeltaTime);
}

void APlayer::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);
}

void APlayer::OnComponentBeginOverlap(Collider* This, Collider* Other)
{
	BoxCollider* ThisBoxCollider = dynamic_cast<BoxCollider*>(This);
	BoxCollider* OtherBoxCollider = dynamic_cast<BoxCollider*>(Other);

	if (ThisBoxCollider == nullptr || OtherBoxCollider == nullptr)
	{
		return;
	}

	AdjustCollisionPos(ThisBoxCollider, OtherBoxCollider);

	if (OtherBoxCollider->GetCollideLayer() == ECollideLayer::Ground)
	{
		TransitionTo(EPlayerState::MoveOnGround);
	}
}

void APlayer::OnComponentEndOverlap(Collider* This, Collider* Other)
{
	BoxCollider* ThisBoxCollider = dynamic_cast<BoxCollider*>(This);
	BoxCollider* OtherBoxCollider = dynamic_cast<BoxCollider*>(Other);

	if (ThisBoxCollider == nullptr || OtherBoxCollider == nullptr)
	{
		return;
	}

	if (OtherBoxCollider->GetCollideLayer() == ECollideLayer::Ground)
	{
		// TODO
	}
}

EPlayerState APlayer::GetCurrentState() const
{
}

void APlayer::TransitionTo(EPlayerState NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	switch (NewState)
	{
	case EPlayerState::MoveOnGround:
		Speed.Y = 0.f;
		break;
	case EPlayerState::Fall:
		break;
	}

	CurrentState = NewState;
}

void APlayer::OnInput(float DeltaTime)
{
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
}

void APlayer::OnMove()
{
	if (InputManager::Get()->GetButtonDown(EKeyCode::Space))
	{
		Jump();
	}
}

void APlayer::OnFall()
{

}

void APlayer::Jump()
{
	if (CurrentState == EPlayerState::Fall)
	{
		return;
	}

	TransitionTo(EPlayerState::Fall);
	Speed.Y = -500.f;
}

void APlayer::OnTickGravity(float DeltaTime)
{
	if (DeltaTime > 0.1f)
	{
		return;
	}

	Speed.Y += Gravity * DeltaTime;
	Position.Y += Speed.Y * DeltaTime;
}

// 진입한 방향 반대로 다시 밀쳐서 해당 칸으로 움직이지 못하도록 보정하는 함수
void APlayer::AdjustCollisionPos(BoxCollider* This, BoxCollider* Other)
{
	const RECT ThisRect = This->GetRect();
	const RECT OtherRect = Other->GetRect();

	RECT Intersect{};
	Vector2 NewPosition = GetPosition();
	if (::IntersectRect(&Intersect, &ThisRect, &OtherRect))
	{
		// Intersect 사각형의 가로가 길면 가로 방향으로, 세로가 길면 세로 방향으로 밂
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
