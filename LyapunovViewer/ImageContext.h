#pragma once
#include <Windows.h>

class ImageContext {
	HBITMAP bitmap = { 0 };
	HDC buffer = { 0 };
	const int width = 512;
	const int height = 512;

public:
	ImageContext() {}
	void initialize(HWND& hWnd);
	

	virtual ~ImageContext() {}

	void Draw(HDC& hdc);

	void Destroy();
};