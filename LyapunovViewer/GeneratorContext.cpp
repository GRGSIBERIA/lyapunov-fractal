#include "GeneratorContext.h"

void GenerateSpoitButton(HWND hWnd, LPARAM& lParam) {
    int i = 0;
    const int width = 50;
    const int space = 8;
#define Button(STR, ID) CreateWindow(\
    TEXT("BUTTON"), TEXT(STR),\
    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,\
    370 + (width + space) * i++, 8, width, 24,\
    hWnd, (HMENU)ID, ((LPCREATESTRUCT)(lParam))->hInstance, NULL\
    )

    Button("a min", BUTTON_ID_SPOIT_AMIN);
    Button("a max", BUTTON_ID_SPOIT_AMAX);
    Button("b min", BUTTON_ID_SPOIT_BMIN);
    Button("b max", BUTTON_ID_SPOIT_BMAX);
    Button("T-L", BUTTON_ID_SPOIT_TL);
    Button("B-R", BUTTON_ID_SPOIT_BR);
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
    c += 1;
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