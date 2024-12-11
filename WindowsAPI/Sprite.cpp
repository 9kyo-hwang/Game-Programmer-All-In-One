#include "pch.h"
#include "Sprite.h"

#include <assert.h>

#include "Texture.h"

Sprite::Sprite(Texture* InTexture, int32 InX, int32 InY, int32 InWidth, int32 InHeight)
	: MyTexture(InTexture)
	, X(InX)
	, Y(InY)
	, Width(InWidth)
	, Height(InHeight)
{
}

Sprite::~Sprite()
{
}

HDC Sprite::GetDC() const
{
	return MyTexture->GetDC();
}

int32 Sprite::GetTransparent() const
{
	return MyTexture->GetTransparent();
}
