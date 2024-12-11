#include "pch.h"
#include "Button.h"

#include "InputManager.h"
#include "Sprite.h"
#include "TimerManager.h"

Button::Button()
{
}

Button::~Button()
{
}

void Button::BeginPlay()
{
	Super::BeginPlay();
	SetButtonState(EButtonState::Default);
}

void Button::Tick()
{
	POINT Mouse = InputManager::Get()->GetMousePosition();
	float DeltaTime = TimerManager::Get()->GetDeltaTime();

	if (State == EButtonState::Clicked)  // 계속 Click 상태인 것을 방지하기 위해
	{
		ElapsedTime += DeltaTime;
		if (ElapsedTime >= 0.2f)  // 0.2초 초과 시 클릭 완료
		{
			ElapsedTime = 0.0f;
			SetButtonState(EButtonState::Default);
		}
	}

	if (IsMouseInRectangle())
	{
		if (InputManager::Get()->GetButton(EKeyCode::LeftMouseButton))
		{
			SetButtonState(EButtonState::Pressed);
			// OnPressed
		}
		else
		{
			if (State == EButtonState::Pressed)
			{
				SetButtonState(EButtonState::Clicked);
				if (OnClick)
				{
					OnClick();
				}
			}
		}
	}
	else
	{
		SetButtonState(EButtonState::Default);
	}
}

void Button::Render(HDC DeviceContextHandle)
{
	if (CurrentSprite)
	{
		// 카메라와 무관하게 계속 띄워야 함
		::TransparentBlt(DeviceContextHandle,
			Position.X - Size.X / 2,
			Position.Y - Size.Y / 2,
			Size.X,
			Size.Y,
			CurrentSprite->GetDC(),
			CurrentSprite->GetPosition().X,
			CurrentSprite->GetPosition().Y,
			CurrentSprite->GetSize().X,
			CurrentSprite->GetSize().Y,
			CurrentSprite->GetTransparent());
	}
	else
	{
		Utils::DrawRectangle(DeviceContextHandle, Position, Size.X, Size.Y);
	}
}

void Button::SetButtonState(EButtonState NewState)
{
	State = NewState;
	if (Sprites[static_cast<int32>(State)])
	{
		SetCurrentSprite(Sprites[static_cast<int32>(State)]);
	}
}
