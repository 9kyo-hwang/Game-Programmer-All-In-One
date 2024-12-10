#include "pch.h"
#include "SpriteRenderer.h"
#include "Sprite.h"

void SpriteRenderer::Start()
{
	Component::Start();
}

void SpriteRenderer::Update()
{
	Component::Update();
}

void SpriteRenderer::Render(HDC InDC)
{
	Component::Render(InDC);

	if (!MySprite)
	{
		return;
	}

	Vector2Int Size = MySprite->GetSize();

	::BitBlt(InDC, 400, 500, Size.X, Size.Y,
		MySprite->GetDC(), MySprite->GetPosition().X, MySprite->GetPosition().Y, SRCCOPY);
}
