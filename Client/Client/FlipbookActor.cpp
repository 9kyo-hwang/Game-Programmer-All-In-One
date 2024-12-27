#include "pch.h"
#include "FlipbookActor.h"

#include "Flipbook.h"
#include "SceneManager.h"
#include "Texture.h"

AFlipbook::AFlipbook()
{
}

AFlipbook::~AFlipbook()
{
}

void AFlipbook::BeginPlay()
{
	Super::BeginPlay();
}

void AFlipbook::Tick(float DeltaTime)
{
	Super::Tick();

	if (!CurrentFlipbook)
	{
		return;
	}

	const FlipbookInfo& Info = CurrentFlipbook->GetInfo();
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

void AFlipbook::Render(HDC DeviceContextHandle)
{
	Super::Render(DeviceContextHandle);

	if (!CurrentFlipbook)
	{
		return;
	}

	Vector2 Camera = SceneManager::Get()->GetCameraPosition();
	const FlipbookInfo& Info = CurrentFlipbook->GetInfo();

	::TransparentBlt(DeviceContextHandle, 
		CurrentPosition.X - Info.Size.X / 2 - (Camera.X - GWinSizeX / 2), CurrentPosition.Y - Info.Size.Y / 2 - (Camera.Y - GWinSizeY / 2),  // ���� ī�޶� ������ �߽��� ��ǥ�� �����Ƿ�
		Info.Size.X, Info.Size.Y,
		Info.MyTexture->GetDC(), 
		(Info.Begin + Index) * Info.Size.X, Info.Line * Info.Size.Y,
		Info.Size.X, Info.Size.Y,
		Info.MyTexture->GetTransparent());
}

void AFlipbook::SetFlipbook(Flipbook* NewFlipbook)
{
	if (NewFlipbook && CurrentFlipbook == NewFlipbook)
	{
		return;
	}

	CurrentFlipbook = NewFlipbook;
	Reset();
}

void AFlipbook::Reset()
{
	ElapsedTime = 0.0f;
	Index = 0;  // �ε����� 0���� �ʱ�ȭ�ϱ� ������ TransparentBlt�� �׸� �� (Begin + Index)���� ����ؾ� ��
}

bool AFlipbook::HasAnimationFinished() const
{
	if (!CurrentFlipbook)
	{
		return true;
	}

	const FlipbookInfo& Info = CurrentFlipbook->GetInfo();
	if (!Info.bLoop && Index == Info.End)
	{
		return true;
	}

	return false;
}
