#pragma once
#include "Resources.h"

class Texture final : public Resources
{
public:
	Texture();
	~Texture() override;

	Texture* Load(HWND Window, const wstring& Path);
	HDC GetDC() const { return DC; }

	Vector2Int GetSize() const { return Size; }
	void SetSize(Vector2Int NewSize) { Size = NewSize; }

	uint32 GetTransparent() const { return Transparent; }
	void SetTransparent(uint32 NewTransparent) { Transparent = NewTransparent; }

private:
	HDC DC{};
	HBITMAP Bmp{};
	Vector2Int Size{};
	uint32 Transparent{RGB(255, 0, 255)};  // �̹����� alpha ä���� ���� ����ȭ��ų ��� ������ �����ؾ� ��
};

