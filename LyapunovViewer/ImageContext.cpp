#include "ImageContext.h"

void ImageContext::initialize(HWND& hWnd, const int width, const int height)
{
	bufW = width;
	bufH = height;
	destroy();

	HDC hdc = GetDC(hWnd);

	bitmap = CreateCompatibleBitmap(hdc, bufW, bufH);
	buffer = CreateCompatibleDC(hdc);

	SelectObject(buffer, bitmap);
	SelectObject(buffer, GetStockObject(NULL_PEN));

	PatBlt(buffer, 0, 0, bufW, bufH, WHITENESS);

	ReleaseDC(hWnd, hdc);
}

#include <vector>
void ImageContext::generate(const EditContext& edit)
{
	using namespace std;
	auto r = vector<vector<vector<float>>>(bufH, vector<vector<float>>(bufW, vector<float>(edit.PN+1)));
	auto x = vector<vector<vector<float>>>(bufH, vector<vector<float>>(bufW, vector<float>(edit.PN+1)));
	const auto N = edit.PN + 1;

#pragma omp parallel for
	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; ++w) {
			for (int n = 1; n < N; ++n) {

			}
		}
	}
}

void ImageContext::draw(HDC& hdc)
{
	SelectObject(buffer, bitmap);

	//BitBlt(hdc, 320 + 24, 42, width, height, buffer, 0, 0, SRCCOPY);
	SetStretchBltMode(hdc, COLORONCOLOR);
	StretchBlt(hdc, 320 + 24, 42, curW, curH, buffer, 0, 0, bufW, bufH, SRCCOPY);

	TextOut(hdc, 324, 300, L"B", lstrlen(L"B"));
	TextOut(hdc, 320 + 256, 560, L"A", lstrlen(L"A"));
	TextOut(hdc, 324 - 8, 32 + 16, L"min", lstrlen(L"min"));
	TextOut(hdc, 324 - 12, 512 + 16, L"max", lstrlen(L"max"));
	TextOut(hdc, 324 + 24, 560, L"min", lstrlen(L"min"));
	TextOut(hdc, 324 + 512 - 12, 560, L"max", lstrlen(L"max"));
}

void ImageContext::destroy()
{
	DeleteObject(bitmap);
}
