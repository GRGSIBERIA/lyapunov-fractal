#pragma once
#include <Windows.h>
#include "Resource.h"

class PlotterContext {
	HWND window;

public:
	void initialize(HINSTANCE& hInst, HWND& hWnd);
};