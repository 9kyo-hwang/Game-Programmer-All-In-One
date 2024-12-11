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
	// ���ø� �渶�� ����(��ü��, ��ü �� �Լ� ������)
	template<typename T>
	void AddOnClickDelegate(T* Owner, void(T::* Func)())
	{
		OnClick = [Owner, Func]()
			{
				(Owner->*Func)();
			};
	}

	// �Լ� ������ + �Լ� ��ü(���� �Լ� ��) ��� Ŀ�� ����
	function<void(void)> OnClick = nullptr;

protected:
	Sprite* CurrentSprite = nullptr;
	Sprite* Sprites[static_cast<int32>(EButtonState::End)]{};  // ��ư ���¿� ���� �ٸ� ���� ����
	EButtonState State = EButtonState::Default;
	float ElapsedTime = 0.0f;
};
