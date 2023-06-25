#include "PlotterContext.h"

LRESULT CALLBACK WndProcSub(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

void PlotterContext::initialize(HINSTANCE& hInst, HWND& hWnd)
{
    WNDCLASSEXW wcex;
    const auto plotterName = TEXT("3D PLOT");

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProcSub;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    //wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LYAPUNOVVIEWER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    //wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LYAPUNOVVIEWER);
    wcex.lpszClassName = plotterName;
    //wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    // RGB(212, 207, 201)
    RegisterClassExW(&wcex);

    wcex.lpfnWndProc = DefMDIChildProc;
    wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcex.lpszClassName = TEXT("MDICHILD");

    RegisterClassExW(&wcex);

	window = CreateWindowW(plotterName, plotterName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWnd, nullptr, hInst, nullptr);
}
