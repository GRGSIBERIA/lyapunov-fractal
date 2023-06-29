#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>

#include "ImageContext.h"
#include "STLGenerator.h"

class POVRAYGenerator {
public:
	POVRAYGenerator(HWND& hWnd, const V2D& lambda, const C2D color, const float width2d, const float height2d, const Space& space);

	virtual ~POVRAYGenerator() {}
};