#pragma once
#include <Windows.h>
#include "Resource.h"
#include "ImageContext.h"

class PlotterStructure {
	float height = 10.f;
	float width = 10.f;
	float depth = 10.f;

public:
	HWND _height;
	HWND _width;
	HWND _depth;
	HWND _thickness;
};

class PlotterContext {
	HWND window;

public:
	void initialize(HINSTANCE& hInst, HWND& hWnd, const ImageContext& image);
};