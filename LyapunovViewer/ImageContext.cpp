#include "ImageContext.h"

void ImageContext::initialize(HWND& hWnd)
{
	HDC hdc = GetDC(hWnd);

	bitmap = CreateCompatibleBitmap(hdc, width, height);
	buffer = CreateCompatibleDC(hdc);

	SelectObject(buffer, bitmap);
	SelectObject(buffer, GetStockObject(NULL_PEN));

	PatBlt(buffer, 0, 0, width, height, WHITENESS);

	ReleaseDC(hWnd, hdc);
}

void ImageContext::Draw(HDC& hdc)
{
	SelectObject(buffer, bitmap);
	BitBlt(hdc, 320, 42, width, height, buffer, 0, 0, SRCCOPY);
}

void ImageContext::Destroy()
{
	DeleteObject(bitmap);
}
