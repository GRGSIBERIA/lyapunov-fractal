#include "PlotterContext.h"
#include <string>

V2D lambda;
C2D pixel;
int bufW;
int bufH;

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
        hWnd, (HMENU)id, ((LPCREATESTRUCT)(lParam))->hInstance, NULL
    );
}

#include "STLGenerator.h"
#include "POVRAYGenerator.h"
#include <streambuf>
LRESULT CALLBACK WndProcSub(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static PlotterStructure plot;
    
    switch (message) {
    case WM_CREATE:
    {
        int c = 0;
#define CEdit(STR) CreateEdit(lParam, hWnd, c, L##STR)

        plot._thickness = CEdit("10.0");
        plot._width = CEdit("200.0");
        plot._height = CEdit("200.0");
        plot._depth = CEdit("200.0");
        
        plot._isMetalic = CreateWindow(
            TEXT("BUTTON"), TEXT("Metalic"),
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
            10, 10 + c++ * 32, 128, 24,
            hWnd, NULL, ((LPCREATESTRUCT)(lParam))->hInstance, NULL
        );


        CreateButton(lParam, hWnd, c, 150, L"GENERATE POVRAY", BUTTON_ID_SUB_GENERATE_POVRAY);
        CreateButton(lParam, hWnd, c, 150, L"GENERATE STL", BUTTON_ID_SUB_GENERATE_STL);

        
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

        TOut("Thickness (mm)");
        TOut("Width (mm)");
        TOut("Height (mm)");
        TOut("Depth (mm)");
        
        EndPaint(hWnd, &ps);
        ReleaseDC(hWnd, hdc);

        break;
    }
    case WM_COMMAND:
    {
        auto func = [](HWND& hWnd, float& val) -> void {
            TCHAR buf[256];
            GetWindowText(hWnd, buf, 256);
            val = std::stof(buf);

            if (!(val > 0.f)) throw std::exception("Undercover Error");
        };

        int wmId = LOWORD(wParam);
        // ëIëÇ≥ÇÍÇΩÉÅÉjÉÖÅ[ÇÃâêÕ:
        switch (wmId) {
        case BUTTON_ID_SUB_GENERATE_POVRAY:
        {
            Space space;
            try {
                func(plot._depth, space.depth);
                func(plot._height, space.height);
                func(plot._width, space.width);
                func(plot._thickness, space.thickness);
            }
            catch (...) {
                MessageBox(NULL, L"Please check if the entered value is correct", L"Invalid Values", MB_OK | MB_ICONERROR);
                break;
            }
            space.isMetal = BST_CHECKED == SendMessage(plot._isMetalic, BM_GETCHECK, 0, 0) ? true : false;

            POVRAYGenerator pov(hWnd, lambda, pixel, bufW, bufH, space);

            break;
        }
        case BUTTON_ID_SUB_GENERATE_STL:
        {
            Space space;
            
            try {
                func(plot._depth, space.depth);
                func(plot._height, space.height);
                func(plot._width, space.width);
                func(plot._thickness, space.thickness);
            }
            catch (...) {
                MessageBox(NULL, L"Please check if the entered value is correct", L"Invalid Values", MB_OK | MB_ICONERROR);
                break;
            }
            
            STLGenerator gen(hWnd, lambda, bufW, bufH, space);

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
    bufW = image.getWidth();
    bufH = image.getHeight();
}
