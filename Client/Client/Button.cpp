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

	if (State == EButtonState::Clicked)  // ��� Click ������ ���� �����ϱ� ����
	{
		ElapsedTime += DeltaTime;
		if (ElapsedTime >= 0.2f)  // 0.2�� �ʰ� �� Ŭ�� �Ϸ�
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
		// ī�޶�� �����ϰ� ��� ����� ��
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
