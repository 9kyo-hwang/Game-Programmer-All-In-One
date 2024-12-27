#include "pch.h"
#include "InputManager.h"

void InputManager::Initialize(HWND InWindowHandle)
{
	WindowHandle = InWindowHandle;
	KeyStates.resize(KEY_CODE_COUNT, EKeyState::None);
}

void InputManager::Update()
{
	// ::GetAsyncKeyState();  // �ش� ����� ���� �� ȣ���� ��� �ӵ��� ���� ������

	BYTE Ascii[KEY_CODE_COUNT]{};
	if (!::GetKeyboardState(Ascii))  // ��� Ű ���¸� �� ���� �������� �Լ�
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
	::ScreenToClient(WindowHandle, &MousePosition);  // window ���� ��ǥ�� ��ȯ
}
