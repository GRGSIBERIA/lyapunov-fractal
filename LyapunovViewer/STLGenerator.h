#pragma once
#include <Windows.h>
#include <commdlg.h>
#include "ImageContext.h"

struct Space {
	float width;
	float height;
	float depth;
	float thickness;
	float min;
	float max;
};

class STLGenerator {
public:
	STLGenerator(HWND& hWnd, const V2D& lambda, const int width, const int height, const Space& space);

	virtual ~STLGenerator() {}
};