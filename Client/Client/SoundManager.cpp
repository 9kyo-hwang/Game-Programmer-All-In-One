#include "pch.h"
#include "SoundManager.h"

#include "ResourceManager.h"
#include "Sound.h"

SoundManager::~SoundManager()
{
	if (Device)
	{
		Device->Release();
	}
}

void SoundManager::Initialize(HWND WindowHandle)
{
	if (FAILED(::DirectSoundCreate(nullptr, &Device, nullptr)))
	{
		::MessageBox(nullptr, L"[SoundManager::Initialize] Failed to Direct Sound Create", L"System Error", MB_OK);
		return;
	}

	if (FAILED(Device->SetCooperativeLevel(WindowHandle, DSSCL_PRIORITY)))
	{
		::MessageBox(nullptr, L"[SoundManager::Initialize] Failed to Set Cooperative Level", L"System Error", MB_OK);
		return;
	}
}

void SoundManager::Play(const wstring& Name, bool bLoop)
{
	if (Sound* TargetSound = ResourceManager::Get()->GetSound(Name))
	{
		TargetSound->Play(bLoop);
	}
}
