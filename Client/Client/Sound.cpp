#include "pch.h"
#include "Sound.h"

#include "SoundManager.h"

Sound::Sound()
{
}

Sound::~Sound()
{
	if (Buffer)
	{
		Buffer->Release();
	}
}

bool Sound::Load(fs::path Path)
{
	if (Path.extension() != L".wav")
		return false;
	
	if (HMMIO File = ::mmioOpen(const_cast<WCHAR*>(Path.c_str()), nullptr, MMIO_READ))
	{
		MMCKINFO Parent = {};
		Parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		::mmioDescend(File, &Parent, nullptr, MMIO_FINDRIFF);

		MMCKINFO Child = {};
		Child.ckid = mmioFOURCC('f', 'm', 't', ' ');
		::mmioDescend(File, &Child, &Parent, MMIO_FINDCHUNK);

		WAVEFORMATEX Data = {};
		::mmioRead(File, reinterpret_cast<char*>(&Data), sizeof(Data));

		::mmioAscend(File, &Child, 0);
		Child.ckid = mmioFOURCC('d', 'a', 't', 'a');
		::mmioDescend(File, &Child, &Parent, MMIO_FINDCHUNK);

		::memset(&Description, 0, sizeof(DSBUFFERDESC));
		Description.dwBufferBytes = Child.cksize;
		Description.dwSize = sizeof(DSBUFFERDESC);
		Description.dwFlags = DSBCAPS_STATIC;
		Description.lpwfxFormat = &Data;

		if (SUCCEEDED(SoundManager::Get()->GetSoundCard()->CreateSoundBuffer(&Description, &Buffer, nullptr)))
		{
			void* AudioPtr1 = nullptr;
			void* AudioPtr2 = nullptr;
			DWORD AudioBytes1;
			DWORD AudioBytes2;

			Buffer->Lock(0, Child.cksize, &AudioPtr1, &AudioBytes1, &AudioPtr2, &AudioBytes2, 0);
			if (AudioPtr1)
			{
				::mmioRead(File, static_cast<char*>(AudioPtr1), AudioBytes1);
			}
			if (AudioPtr2)
			{
				::mmioRead(File, static_cast<char*>(AudioPtr2), AudioBytes2);
			}

			Buffer->Unlock(AudioPtr1, AudioBytes1, AudioPtr2, AudioBytes2);

			::mmioClose(File, 0);
			return true;
		}

		::MessageBox(nullptr, L"[Sound::Load] Failed to Create Sound Buffer", L"", MB_OK);
		return false;
	}

	::MessageBox(nullptr, L"[Sound::Load] No Sound File", L"", MB_OK);
	return false;
}

void Sound::Play(bool bLoop)
{
	assert(Buffer != nullptr);

	Buffer->SetCurrentPosition(0);
	bLoop
	? Buffer->Play(0, 0, DSBPLAY_LOOPING)
	: Buffer->Play(0, 0, 0);
}

void Sound::Stop(bool bReset)
{
	assert(Buffer != nullptr);

	Buffer->Stop();
	if (bReset)
	{
		Buffer->SetCurrentPosition(0);
	}
}

void Sound::Load(const wstring& Path)
{
	Resources::Load(Path);
}

void Sound::Save(const wstring& Path)
{
	Resources::Save(Path);
}
