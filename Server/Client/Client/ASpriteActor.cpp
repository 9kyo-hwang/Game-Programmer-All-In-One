#include "pch.h"
#include "ASpriteActor.h"

#include "SceneManager.h"
#include "Sprite.h"

ASprite::ASprite()
{
}

ASprite::~ASprite()
{
}

void ASprite::BeginPlay()
{
	Super::BeginPlay();
}

void ASprite::Tick(float DeltaTime)
{
	Super::Tick();
}

void ASprite::Render(HDC InDC)
{
	Super::Render(InDC);

	if (!MySprite)
	{
		return;
	}

	Vector2Int Size = MySprite->GetSize();
	Vector2 Camera = SceneManager::Get()->GetCameraPosition();

	// 배경은 굳이 Transparent를 고려할 필요가 없어서 속도가 더 빠른 BitBlt 사용
	::BitBlt(InDC,
		CurrentPosition.X - Size.X / 2 - (Camera.X - GWinSizeX / 2), CurrentPosition.Y - Size.Y / 2 - (Camera.Y - GWinSizeY / 2),
		Size.X, Size.Y,
		MySprite->GetDC(),
		MySprite->GetPosition().X, MySprite->GetPosition().Y, 
		SRCCOPY);
}
