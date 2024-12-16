#include "pch.h"
#include "ASpriteActor.h"

#include "SceneManager.h"
#include "Sprite.h"

ASpriteActor::ASpriteActor()
{
}

ASpriteActor::~ASpriteActor()
{
}

void ASpriteActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASpriteActor::Tick(float DeltaTime)
{
	Super::Tick();
}

void ASpriteActor::Render(HDC InDC)
{
	Super::Render(InDC);

	if (!MySprite)
	{
		return;
	}

	Vector2Int Size = MySprite->GetSize();
	Vector2 Camera = SceneManager::Get()->GetCameraPosition();

	// ����� ���� Transparent�� ����� �ʿ䰡 ��� �ӵ��� �� ���� BitBlt ���
	::BitBlt(InDC,
		CurrentPosition.X - Size.X / 2 - (Camera.X - GWinSizeX / 2), CurrentPosition.Y - Size.Y / 2 - (Camera.Y - GWinSizeY / 2),
		Size.X, Size.Y,
		MySprite->GetDC(),
		MySprite->GetPosition().X, MySprite->GetPosition().Y, 
		SRCCOPY);
}
