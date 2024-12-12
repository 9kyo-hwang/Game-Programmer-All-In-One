#include "pch.h"
#include "FlipbookActor.h"

#include "Flipbook.h"
#include "SceneManager.h"
#include "Texture.h"

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

void FlipbookActor::Tick(float DeltaTime)
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

	ElapsedTime += DeltaTime;

	int32 NumFrame = Info.End - Info.Begin + 1;
	float FrameTime = Info.Duration / NumFrame;
	if (ElapsedTime >= FrameTime)
	{
		ElapsedTime = 0.0f;
		Index = (Index + 1) % NumFrame;  // ������ ������ ���� �� 0���� �ʱ�ȭ
	}
}

void FlipbookActor::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);

	if (!MyFlipbook)
	{
		return;
	}

	Vector2 Camera = SceneManager::Get()->GetCameraPosition();
	const FlipbookInfo& Info = MyFlipbook->GetInfo();

	::TransparentBlt(DeviceContextHandle, 
		Position.X - Info.Size.X / 2 - (Camera.X - GWinSizeX / 2), Position.Y - Info.Size.Y / 2 - (Camera.Y - GWinSizeY / 2),  // ���� ī�޶� ������ �߽��� ��ǥ�� �����Ƿ�
		Info.Size.X, Info.Size.Y,
		Info.MyTexture->GetDC(), 
		(Info.Begin + Index) * Info.Size.X, Info.Line * Info.Size.Y,
		Info.Size.X, Info.Size.Y,
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
	Index = 0;  // �ε����� 0���� �ʱ�ȭ�ϱ� ������ TransparentBlt�� �׸� �� (Begin + Index)���� ����ؾ� ��
}
