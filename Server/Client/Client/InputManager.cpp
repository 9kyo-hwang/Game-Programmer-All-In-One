#include "pch.h"
#include "InputManager.h"

void InputManager::Initialize(HWND InWindowHandle)
{
	WindowHandle = InWindowHandle;
	KeyStates.resize(KEY_CODE_COUNT, EKeyState::None);
}

void InputManager::Update()
{
	// ::GetAsyncKeyState();  // 해당 방식은 여러 번 호출할 경우 속도가 많이 느려짐

	BYTE Ascii[KEY_CODE_COUNT]{};
	if (!::GetKeyboardState(Ascii))  // 모든 키 상태를 한 번에 가져오는 함수
	{
		return;
	}

	for (uint32 Key = 0; Key < KEY_CODE_COUNT; ++Key)
	{
		EKeyState& State = KeyStates[Key];
		if (Ascii[Key] & 0x80)
		{
			if (State == EKeyState::Hold || State == EKeyState::Down)
			{
				State = EKeyState::Hold;
			}
			else
			{
				State = EKeyState::Down;
			}
		}
		else
		{
			if (State == EKeyState::Hold || State == EKeyState::Down)
			{
				State = EKeyState::Up;
			}
			else
			{
				State = EKeyState::None;
			}
		}
	}

	::GetCursorPos(&MousePosition);
	::ScreenToClient(WindowHandle, &MousePosition);  // window 기준 좌표로 변환
}
