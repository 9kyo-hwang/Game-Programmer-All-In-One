#pragma once
#include "Singleton.h"

// ���� ī�带 ��ǥ�ϴ� ��ü
class SoundManager : public Singleton<SoundManager>
{
public:
	~SoundManager() override;

	void Initialize(HWND InWindowHandle);
	void Play(const wstring& Name, bool bLoop = false);
	LPDIRECTSOUND GetSoundCard() const { return Device; }

private:
	LPDIRECTSOUND Device = nullptr;
};

