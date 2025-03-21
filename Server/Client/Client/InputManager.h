#pragma once
#include "Singleton.h"

// MS에서 제공하는 VK_ 매크로는 외우기 힘들어 한 번 더 래핑
enum class EKeyCode : uint8
{
	None,

	LeftMouseButton = VK_LBUTTON,
	RightMouseButton = VK_RBUTTON,

	UpArrow = VK_UP,
	DownArrow = VK_DOWN,
	LeftArrow = VK_LEFT,
	RightArrow = VK_RIGHT,
	Space = VK_SPACE,

	Alpha0 = '0',
	Alpha1 = '1',
	Alpha2 = '2',
	Alpha3 = '3',
	Alpha4 = '4',
	Alpha5 = '5',
	Alpha6 = '6',
	Alpha7 = '7',
	Alpha8 = '8',
	Alpha9 = '9',

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',
	Q = 'Q',
	E = 'E'
};

enum class EKeyState : uint8
{
	None,
	Down,
	Hold,
	Up,
	End
};

constexpr int32 KEY_CODE_COUNT = static_cast<int32>(UINT8_MAX) + 1;
constexpr int32 KEY_STATE_COUNT = static_cast<int32>(EKeyState::End);

class InputManager : public Singleton<InputManager>
{
public:
	~InputManager() override {}
	void Initialize(HWND InWindowHandle);
	void Update();

	bool GetButton(EKeyCode Key) const { return GetKeyState(Key) == EKeyState::Hold; }
	bool GetButtonDown(EKeyCode Key) const { return GetKeyState(Key) == EKeyState::Down; }
	bool GetButtonUp(EKeyCode Key) const { return GetKeyState(Key) == EKeyState::Up; }
	POINT GetMousePosition() const { return MousePosition; }

private:
	EKeyState GetKeyState(EKeyCode Key) const { return KeyStates[static_cast<uint8>(Key)]; }

private:
	HWND WindowHandle = nullptr;
	vector<EKeyState> KeyStates;
	POINT MousePosition{};
};

