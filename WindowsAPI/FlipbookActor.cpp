#include "pch.h"
#include "FlipbookActor.h"

#include "Flipbook.h"
#include "Texture.h"
#include "TimerManager.h"

FlipbookActor::FlipbookActor()
{
}

FlipbookActor::~FlipbookActor()
{
}

void FlipbookActor::BeginPlay()
{
	Super::BeginPlay();
}

void FlipbookActor::Tick()
{
	Super::Tick();

	if (!MyFlipbook)
	{
		return;
	}

	const FlipbookInfo& Info = MyFlipbook->GetInfo();
	if (!Info.bLoop && Index == Info.End)
	{
		return;
	}

	float DeltaTime = TimerManager::Get()->GetDeltaTime();
	ElapsedTime += DeltaTime;

	int32 NumFrame = Info.End - Info.Begin + 1;
	float FrameTime = Info.Duration / NumFrame;
	if (ElapsedTime >= FrameTime)
	{
		ElapsedTime = 0.0f;
		Index = (Index + 1) % NumFrame;  // 마지막 프레임 도달 시 0으로 초기화
	}
}

void FlipbookActor::Render(HDC InDC)
{
	Super::Render(InDC);

	if (!MyFlipbook)
	{
		return;
	}

	const FlipbookInfo& Info = MyFlipbook->GetInfo();
	::TransparentBlt(InDC, Position.X - Info.Size.X / 2, Position.Y - Info.Size.Y / 2, Info.Size.X, Info.Size.Y,
		Info.MyTexture->GetDC(), (Info.Begin + Index) * Info.Size.X, Info.Line * Info.Size.Y, Info.Size.X, Info.Size.Y,
		Info.MyTexture->GetTransparent());
}

void FlipbookActor::SetFlipbook(Flipbook* NewFlipbook)
{
	if (NewFlipbook && MyFlipbook == NewFlipbook)
	{
		return;
	}

	MyFlipbook = NewFlipbook;
	Reset();
}

void FlipbookActor::Reset()
{
	ElapsedTime = 0.0f;
	Index = 0;  // 인덱스를 0으로 초기화하기 때문에 TransparentBlt로 그릴 때 (Begin + Index)으로 계산해야 함
}
