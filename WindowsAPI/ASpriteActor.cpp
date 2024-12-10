#include "pch.h"
#include "ASpriteActor.h"

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

void ASpriteActor::Tick()
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

	//::BitBlt(InDC, Position.X - Size.X / 2, Position.Y - Size.Y / 2, Size.X, Size.Y,
	//	MySprite->GetDC(), MySprite->GetPosition().X, MySprite->GetPosition().Y, SRCCOPY);

	// 투명 처리를 해주는 함수
	::TransparentBlt(InDC, Position.X - Size.X / 2, Position.Y - Size.Y / 2, Size.X, Size.Y,
		MySprite->GetDC(), MySprite->GetPosition().X, MySprite->GetPosition().Y, MySprite->GetSize().X, MySprite->GetSize().Y, MySprite->GetTransparent());
}
