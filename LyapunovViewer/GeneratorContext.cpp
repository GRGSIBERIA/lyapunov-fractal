#include "GeneratorContext.h"

void GenerateSpoitButton(HWND hWnd, LPARAM& lParam) {
    int i = 0;
    const int width = 50;
    const int space = 8;
    CreateWindow(
        TEXT("BUTTON"), TEXT("a min"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        370 + (width + space) * i++, 8, width, 24,
        hWnd, (HMENU)BUTTON_ID_SPOIT_AMIN, ((LPCREATESTRUCT)(lParam))->hInstance, NULL
    );
    CreateWindow(
        TEXT("BUTTON"), TEXT("a max"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        370 + (width + space) * i++, 8, width, 24,
        hWnd, (HMENU)BUTTON_ID_SPOIT_AMAX, ((LPCREATESTRUCT)(lParam))->hInstance, NULL
    );
    CreateWindow(
        TEXT("BUTTON"), TEXT("b min"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        370 + (width + space) * i++, 8, width, 24,
        hWnd, (HMENU)BUTTON_ID_SPOIT_BMIN, ((LPCREATESTRUCT)(lParam))->hInstance, NULL
    );
    CreateWindow(
        TEXT("BUTTON"), TEXT("b max"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        370 + (width + space) * i++, 8, width, 24,
        hWnd, (HMENU)BUTTON_ID_SPOIT_BMAX, ((LPCREATESTRUCT)(lParam))->hInstance, NULL
    );
    CreateWindow(
        TEXT("BUTTON"), TEXT("T-L"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        370 + (width + space) * i++, 8, width, 24,
        hWnd, (HMENU)BUTTON_ID_SPOIT_TL, ((LPCREATESTRUCT)(lParam))->hInstance, NULL
    );
    CreateWindow(
        TEXT("BUTTON"), TEXT("B-R"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        370 + (width + space) * i++, 8, width, 24,
        hWnd, (HMENU)BUTTON_ID_SPOIT_BR, ((LPCREATESTRUCT)(lParam))->hInstance, NULL
    );
}

void LabelOut(HDC& hdc, const int x, const int y, LPCWSTR label)
{
    TextOut(hdc, x, y, label, lstrlen(label));
    MoveToEx(hdc, x, y + 24, NULL);
    LineTo(hdc, 300, y + 24);
}

void GenerateLabels(HDC& hdc)
{
    int c = 0;
    LabelOut(hdc, 40, 80 + 32 * c++, L"Width");
    LabelOut(hdc, 40, 80 + 32 * c++, L"Height");
    LabelOut(hdc, 40, 80 + 32 * c++, L"Sequence");
    LabelOut(hdc, 40, 80 + 32 * c++, L"Number of iterations");
    LabelOut(hdc, 40, 80 + 32 * c++, L"Initial x value");
    LabelOut(hdc, 40, 80 + 32 * c++, L"a_min");
    LabelOut(hdc, 40, 80 + 32 * c++, L"a_max");
    LabelOut(hdc, 40, 80 + 32 * c++, L"b_min");
    LabelOut(hdc, 40, 80 + 32 * c++, L"b_max");
    LabelOut(hdc, 40, 80 + 32 * c++, L"func");
    LabelOut(hdc, 40, 80 + 32 * c++, L"constance 1");
    LabelOut(hdc, 40, 80 + 32 * c++, L"constance 2");
}