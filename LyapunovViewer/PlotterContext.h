#pragma once
#include <Windows.h>
#include "Resource.h"
#include "ImageContext.h"

class PlotterStructure {
	float height = 200.f;
	float width = 200.f;
	float depth = 10.f;

public:
	HWND _height = NULL;
	HWND _width = NULL;
	HWND _depth = NULL;
	HWND _thickness = NULL;
	HWND _isMetalic = NULL;
};

class PlotterContext {
	HWND window = NULL;

public:
	void initialize(HINSTANCE& hInst, HWND& hWnd, const ImageContext& image);
};