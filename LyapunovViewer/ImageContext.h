#pragma once
#include <Windows.h>
#include "EditContexts.h"
#include <vector>

class ImageContext {
	HBITMAP bitmap = { 0 };
	HDC buffer = { 0 };
	const int curW = 512;
	const int curH = 512;
	int bufW = 512;
	int bufH = 512;
	float lammin = 0.f;
	float lammax = 0.f;
	COLORREF mincolor = RGB(0,0,0);
	COLORREF maxcolor = RGB(255, 255, 255);
	bool isChaos = true;

	// generateÇ≈Ç‡èâä˙âª
	std::vector<std::vector<float>> lambda =
		std::vector<std::vector<float>>(512, std::vector<float>(512, 0.f));
	std::vector<std::vector<COLORREF>> pixcels =
		std::vector<std::vector<COLORREF>>(512, std::vector<COLORREF>(512, 0.f));

public:
	ImageContext() {}
	void initialize(HWND& hWnd, const int width, const int height);
	void generate(HWND& hWnd, const EditContext& edit);

	virtual ~ImageContext() {}

	void draw(HDC& hdc, HWND& hWnd);

	void destroy();

	void setMinColor(const COLORREF color) { mincolor = color; }
	void setMaxColor(const COLORREF color) { maxcolor = color; }
	void setIsChaos(const bool chaos) { isChaos = chaos; }

	
};