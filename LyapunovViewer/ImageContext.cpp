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
	BitBlt(hdc, 320 + 24, 42, width, height, buffer, 0, 0, SRCCOPY);

	TextOut(hdc, 324, 300, L"B", lstrlen(L"B"));
	TextOut(hdc, 320 + 256, 560, L"A", lstrlen(L"A"));
	TextOut(hdc, 324 - 8, 32 + 16, L"min", lstrlen(L"min"));
	TextOut(hdc, 324 - 12, 512 + 16, L"max", lstrlen(L"max"));
	TextOut(hdc, 324 + 24, 560, L"min", lstrlen(L"min"));
	TextOut(hdc, 324 + 512 - 12, 560, L"max", lstrlen(L"max"));
}

void ImageContext::Destroy()
{
	DeleteObject(bitmap);
}
