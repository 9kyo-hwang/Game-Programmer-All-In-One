#pragma once
#include "Resources.h"

enum class ESoundType
{
	BGM,
	Normal,
};

class Sound : public Resources
{
public:
	Sound();
	~Sound() override;

	bool Load(fs::path Path);
	void Play(bool bLoop = false);
	void Stop(bool bReset = false);

private:
	void Load(const wstring& Path) override;
	void Save(const wstring& Path) override;

	LPDIRECTSOUNDBUFFER Buffer = nullptr;
	DSBUFFERDESC Description{};
};

