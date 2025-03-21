#pragma once
#include "Singleton.h"

class TimerManager : public Singleton<TimerManager>
{
public:
	~TimerManager() override {}

	void Initialize();
	void Update();

	uint32 GetFPS() const { return FPS; }
	float GetDeltaTime() const { return DeltaTime; }

private:
	uint64 Frequency = 0;
	uint64 PrevCount = 0;
	float DeltaTime = 0.0f;

	uint32 FrameCount = 0;
	float FrameTime = 0.0f;
	uint32 FPS = 0;
};

