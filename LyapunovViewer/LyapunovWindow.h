#pragma once
#include <Windows.h>
#include <string>

LRESULT CALLBACK LyapunovWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class LyapunovWindow {
	HWND sub;
	WNDCLASSEXW wcex;
	std::wstring windowClass = L"Lyapunov Fractal";

public:
	void initWindow(HINSTANCE hInstance, HWND parent);
};