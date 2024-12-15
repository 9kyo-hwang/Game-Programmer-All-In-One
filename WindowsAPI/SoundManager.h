#pragma once
#include "Singleton.h"

// 사운드 카드를 대표하는 객체
class SoundManager : public Singleton<SoundManager>
{
public:
	~SoundManager();

	void Initialize(HWND WindowHandle);
	void Play(const wstring& Name, bool bLoop = false);
	LPDIRECTSOUND GetSoundCard() const { return Device; }

private:
	LPDIRECTSOUND Device = nullptr;
};

