#include "PlotterContext.h"
#include <string>

V2D lambda;
C2D pixel;

HWND CreateEdit(LPARAM& lParam, HWND& hWnd, int& c, const std::wstring& str) {
    return CreateWindow(
        TEXT("EDIT"), str.c_str(),
        WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
        128, 8 + c++ * 32, 64, 22, hWnd, (HMENU)1,
        ((LPCREATESTRUCT)(lParam))->hInstance, NULL
    );
}

void CreateButton(LPARAM& lParam, HWND& hWnd, int& c, const int width, const std::wstring& str, int id) {
    CreateWindow(
        TEXT("BUTTON"), str.c_str(),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 10 + c++ * 32, width, 24,
        hWnd, (HMENU)BUTTON_ID_OPEN_FILE_DIALOG, ((LPCREATESTRUCT)(lParam))->hInstance, NULL
    );
}

LRESULT CALLBACK WndProcSub(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static PlotterStructure plot;
    
    switch (message) {
    case WM_CREATE:
    {
        int c = 0;
#define CEdit(STR) CreateEdit(lParam, hWnd, c, L##STR)

        plot._width = CEdit("10.0");
        plot._height = CEdit("10.0");
        plot._depth = CEdit("10.0");
        
        CreateButton(lParam, hWnd, c, 128, L"SET MIN COLOR", BUTTON_ID_SUB_SET_MIN_COLOR);
        CreateButton(lParam, hWnd, c, 128, L"SET MAX COLOR", BUTTON_ID_SUB_SET_MAX_COLOR);

        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = GetDC(hWnd);
        BeginPaint(hWnd, &ps);

        SetBkColor(hdc, RGB(192, 192, 192));

        int c = 0;
#define TOut(STR) TextOut(hdc, 10, 10 + c++ * 32, TEXT(STR), lstrlen(TEXT(STR)))

        TOut("Width (mm)");
        TOut("Height (mm)");
        TOut("Depth (mm)");
        
        EndPaint(hWnd, &ps);
        ReleaseDC(hWnd, hdc);

        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // ëIëÇ≥ÇÍÇΩÉÅÉjÉÖÅ[ÇÃâêÕ:
        switch (wmId) {
        case BUTTON_ID_SUB_SET_MIN_COLOR:
        {
            break;
        }
        case BUTTON_ID_SUB_SET_MAX_COLOR:
        {
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    }
    case WM_DESTROY:
    {
        DestroyWindow(hWnd);
        break;
    }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void PlotterContext::initialize(HINSTANCE& hInst, HWND& hWnd, const ImageContext& image)
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

    RECT rect;

    GetClientRect(window, &rect);
    SetWindowPos(window, NULL, rect.left, rect.top, 480, 480, (SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE));

    ShowWindow(window, SW_SHOW);
    //UpdateWindow(window);

    lambda = image.getLambda();
    pixel = image.getPixel();
}
