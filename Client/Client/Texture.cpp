#include "pch.h"
#include "Texture.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

Texture* Texture::LoadTexture(HWND Window, const wstring& Path)
{
	HDC WindowDC = ::GetDC(Window);
	DC = ::CreateCompatibleDC(WindowDC);
	Bmp = static_cast<HBITMAP>(::LoadImage(nullptr, Path.c_str(), IMAGE_BITMAP, 
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION));

	if (!Bmp)
	{
		::MessageBox(Window, Path.c_str(), L"Image Load Failed", 0);
	}

	HBITMAP PrevBmp = static_cast<HBITMAP>(::SelectObject(DC, Bmp));
	::DeleteObject(PrevBmp);

	BITMAP BmpInfo{};
	::GetObject(Bmp, sizeof(BITMAP), &BmpInfo);

	Size = { BmpInfo.bmWidth, BmpInfo.bmHeight };

	return this;
}
