#pragma once
#include "Resources.h"

class Texture;
struct FlipbookInfo
{
	Texture* MyTexture = nullptr;
	wstring Name{};
	Vector2Int Size{};
	int32 Begin;
	int32 End;
	int32 Line;
	float Duration = 1.0f;
	bool bLoop = true;
};

// Unity의 Animation에 대응
class Flipbook : public Resources
{
public:
	Flipbook();
	~Flipbook() override;

	const FlipbookInfo& GetInfo() { return Info; }
	void SetInfo(const FlipbookInfo& NewInfo) { Info = NewInfo; }

private:
	FlipbookInfo Info;
};

