#pragma once
#include <Windows.h>
#include "EditContexts.h"
#include <vector>
#include <intrin.h>

typedef std::vector<std::vector<std::vector<__declspec(align(32)) float>>> V3D;
typedef std::vector<std::vector<__declspec(align(32)) float>> V2D;
typedef std::vector<__declspec(align(32)) float> V1D;
typedef std::vector<std::vector<COLORREF>> C2D;
typedef std::vector<COLORREF> C1D;

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
	V2D lambda = V2D(512, V1D(512, 0.f));
	C2D pixels = C2D(512, C1D(512, 0));

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

	const V2D& getLambda() const { return lambda; }
	const C2D& getPixel() const { return pixels; }

	const int getHeight() const { return bufH; }
	const int getWidth() const { return bufW; }
};