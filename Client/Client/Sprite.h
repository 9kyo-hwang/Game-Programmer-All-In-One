#pragma once
#include "Resources.h"

class Texture;

class Sprite final : public Resources
{
public:
	Sprite(Texture* InTexture, int32 InX, int32 InY, int32 InWidth, int32 InHeight);
	~Sprite() override;

	HDC GetDC() const;
	int32 GetTransparent() const;

	Vector2Int GetPosition() const { return { X, Y }; }
	Vector2Int GetSize() const { return { Width, Height }; }

private:
	Texture* MyTexture = nullptr;
	int32 X, Y, Width, Height;
};

