#pragma once
#include <Windows.h>
#include "Resource.h"

class PreferenceContext {
	HWND _isChaos = NULL;

	COLORREF mincolor = RGB(0, 0, 0);
	COLORREF maxcolor = RGB(255, 255, 255);

	CHOOSECOLOR minchoose = { 0 };
	CHOOSECOLOR maxchoose = { 0 };
	COLORREF custColors[16];

	void initializeChoose(HWND& hWnd, CHOOSECOLOR& choose, COLORREF color);

public:
	PreferenceContext() {}

	virtual ~PreferenceContext() {}

	void initialize(HWND &hWnd, HINSTANCE& hInstance);
	void draw(HDC& hdc);

	const bool isChaos() const;

	enum Choose{
		MIN,
		MAX
	};

	const bool chooseColor(HWND& hWnd, const Choose& chooser);

	const COLORREF getMinColor() const { return minchoose.rgbResult; }
	const COLORREF getMaxColor() const { return maxchoose.rgbResult; }
};