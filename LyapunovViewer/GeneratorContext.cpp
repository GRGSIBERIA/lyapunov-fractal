#include "GeneratorContext.h"

void GenerateSpoitButton(HWND hWnd, LPARAM& lParam) {
    int i = 0;
    const int width = 58;
    const int space = 8;

#define Button(STR, ID) CreateWindow(\
    TEXT("BUTTON"), TEXT(STR),\
    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,\
    370 + (width + space) * i++, 8, width, 24,\
    hWnd, (HMENU)ID, ((LPCREATESTRUCT)(lParam))->hInstance, NULL\
    )

    Button("HOME", BUTTON_ID_SPOIT_HOME);
    Button("a min", BUTTON_ID_SPOIT_AMIN);
    Button("a max", BUTTON_ID_SPOIT_AMAX);
    Button("b min", BUTTON_ID_SPOIT_BMIN);
    Button("b max", BUTTON_ID_SPOIT_BMAX);
    Button("T-L", BUTTON_ID_SPOIT_TL);
    Button("B-R", BUTTON_ID_SPOIT_BR);
    Button("Cancel", BUTTON_ID_SPOIT_CANCEL);
}

void LabelOut(HDC& hdc, const int x, const int y, LPCWSTR label)
{
    TextOut(hdc, x, y, label, lstrlen(label));
    MoveToEx(hdc, x, y + 24, NULL);
    LineTo(hdc, 300, y + 24);
}

void ValidateText(HWND& hWnd, HDC& hdc, const int c, const bool valid) {
    if (valid) SetTextColor(hdc, RGB(0, 0, 0));
    else SetTextColor(hdc, RGB(255, 192, 192));
    RECT r;
    r.left = 40;
    r.top = 80 + 32 * c;
    r.bottom = 40 + 32;
    r.right = 80 + 32 * c + 200;
    InvalidateRect(hWnd, &r, FALSE);
}

void GenerateLabels(HWND& hWnd, HDC& hdc, const EditContext& edit)
{
    int c = 0;

    LabelOut(hdc, 40, 80 + 32 * c++, L"Width");
    ValidateText(hWnd, hdc, c, edit.VHeight);
    LabelOut(hdc, 40, 80 + 32 * c++, L"Height");
    ValidateText(hWnd, hdc, c, edit.VSequence);
    LabelOut(hdc, 40, 80 + 32 * c++, L"Sequence");
    c += 1;
    ValidateText(hWnd, hdc, c, edit.VN);
    LabelOut(hdc, 40, 80 + 32 * c++, L"Number of iterations");
    ValidateText(hWnd, hdc, c, edit.VInitX);
    LabelOut(hdc, 40, 80 + 32 * c++, L"Initial x value");
    ValidateText(hWnd, hdc, c, edit.VAmin);
    LabelOut(hdc, 40, 80 + 32 * c++, L"a_min");
    ValidateText(hWnd, hdc, c, edit.VAmax);
    LabelOut(hdc, 40, 80 + 32 * c++, L"a_max");
    ValidateText(hWnd, hdc, c, edit.VBmin);
    LabelOut(hdc, 40, 80 + 32 * c++, L"b_min");
    ValidateText(hWnd, hdc, c, edit.VBmax);
    LabelOut(hdc, 40, 80 + 32 * c++, L"b_max");
    ValidateText(hWnd, hdc, c, edit.VFunc);
    LabelOut(hdc, 40, 80 + 32 * c++, L"func");
    ValidateText(hWnd, hdc, c, edit.VConst1);
    LabelOut(hdc, 40, 80 + 32 * c++, L"constance 1");
    ValidateText(hWnd, hdc, c, edit.VConst2);
    LabelOut(hdc, 40, 80 + 32 * c++, L"constance 2");

    SetTextColor(hdc, RGB(0, 0, 0));
    InvalidateRect(hWnd, NULL, FALSE);
}