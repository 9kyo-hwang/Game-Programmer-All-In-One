#pragma once
#include "UI.h"

class Sprite;

enum class EButtonState : uint8
{
	Default,
	//Hovered,
	Pressed,
	Clicked,
	End
};

class Button : public UI
{
	using Super = UI;
	using ThisClass = Button;

public:
	Button();
	~Button() override;

	void BeginPlay() override;
	void Tick() override;
	void Render(HDC DeviceContextHandle) override;

	Sprite* GetSprite(EButtonState InState) const { return Sprites[static_cast<int32>(InState)]; }
	void SetCurrentSprite(Sprite* NewSprite) { CurrentSprite = NewSprite; }
	void SetSprite(Sprite* InSprite, EButtonState InState) { Sprites[static_cast<int32>(InState)] = InSprite; }
	void SetButtonState(EButtonState NewState);

public:
	// 템플릿 흑마법 버전(객체와, 객체 내 함수 포인터)
	template<typename T>
	void AddOnClickDelegate(T* Owner, void(T::* Func)())
	{
		OnClick = [Owner, Func]()
			{
				(Owner->*Func)();
			};
	}

	// 함수 포인터 + 함수 객체(람다 함수 등) 모두 커버 가능
	function<void(void)> OnClick = nullptr;

protected:
	Sprite* CurrentSprite = nullptr;
	Sprite* Sprites[static_cast<int32>(EButtonState::End)]{};  // 버튼 상태에 따라 다를 수도 있음
	EButtonState State = EButtonState::Default;
	float ElapsedTime = 0.0f;
};
