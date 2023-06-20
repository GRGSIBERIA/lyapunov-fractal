#pragma once
#include <Windows.h>
#include "EditContexts.h"

class ImageContext {
	HBITMAP bitmap = { 0 };
	HDC buffer = { 0 };
	const int curW = 512;
	const int curH = 512;
	int bufW = 512;
	int bufH = 512;

public:
	ImageContext() {}
	void initialize(HWND& hWnd, const int width, const int height);
	void generate(const EditContext& edit);

	virtual ~ImageContext() {}

	void draw(HDC& hdc);

	void destroy();
};