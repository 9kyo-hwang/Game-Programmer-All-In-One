#include "pch.h"
#include "TimerManager.h"

void TimerManager::Initialize()
{
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&Frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&PrevCount));
}

void TimerManager::Update()
{
	uint64 CurrentCount{};
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&CurrentCount));

	DeltaTime = (CurrentCount - PrevCount) / static_cast<float>(Frequency);
	PrevCount = CurrentCount;

	FrameCount++;
	FrameTime += DeltaTime;

	if (FrameTime >= 1.0f)
	{
		FPS = static_cast<uint32>(FrameCount / FrameTime);

		FrameTime = 0.0f;
		FrameCount = 0;
	}
}