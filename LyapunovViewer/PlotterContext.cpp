#include "PlotterContext.h"

LRESULT CALLBACK WndProcSub(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
    {
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = GetDC(hWnd);
        BeginPaint(hWnd, &ps);

        TextOut(hdc, 10, 10, TEXT("Thickness"), lstrlen(TEXT("Thickness")));

        EndPaint(hWnd, &ps);
        ReleaseDC(hWnd, hdc);

        break;
    }
    case WM_DESTROY:
    {
        CloseWindow(hWnd);
        DestroyWindow(hWnd);
        break;
    }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void PlotterContext::initialize(HINSTANCE& hInst, HWND& hWnd)
{
    const auto plotterName = TEXT("3D PLOT WINDOW");

    WNDCLASS winc;

    winc.style = CS_HREDRAW | CS_VREDRAW;
    winc.lpfnWndProc = WndProcSub;
    winc.cbClsExtra = winc.cbWndExtra = 0;
    winc.hInstance = hInst;
    winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    winc.hCursor = LoadCursor(NULL, IDC_ARROW);
    winc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    winc.lpszMenuName = NULL;
    winc.lpszClassName = TEXT("STATIC");

    if (!RegisterClass(&winc)) return;

	window = CreateWindowW(TEXT("STATIC"), plotterName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInst, nullptr);

    ShowWindow(window, SW_SHOW);
    //UpdateWindow(window);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        DispatchMessage(&msg);
    }
}
