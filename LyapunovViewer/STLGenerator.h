#pragma once
#include <Windows.h>
#include <commdlg.h>
#include "ImageContext.h"

const std::wstring OpenSaveDialog(HWND& hWnd, const std::wstring& filter);

struct Space {
	float width;
	float height;
	float depth;
	float thickness;
	bool isMetal = false;
};

class STLGenerator {
public:
	STLGenerator(HWND& hWnd, const V2D& lambda, const int width, const int height, const Space& space);

	virtual ~STLGenerator() {}
};