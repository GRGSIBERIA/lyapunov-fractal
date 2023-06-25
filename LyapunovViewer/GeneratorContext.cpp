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
    RECT r{ 
        12,
        80 + 32 * c + 5,
        28,
        80 + 32 * (c + 1) - 15
    };
    
    if (valid) {
        SelectObject(hdc, CreateSolidBrush(RGB(0, 0xff, 0)));
    }
    else {
        SelectObject(hdc, CreateSolidBrush(RGB(0xff, 0, 0)));
    }

    RoundRect(hdc, r.left, r.top, r.right, r.bottom, 16, 16);
    DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
}

void GenerateLabels(HWND& hWnd, HDC& hdc, const EditContext& edit)
{
    int c = 0;

#define VALIDATE(NAME) ValidateText(hWnd, hdc, c, edit.##NAME)
#define LABELOUT(WSTR) LabelOut(hdc, 40, 80 + 32 * c++, WSTR);

    VALIDATE(VWidth);
    LABELOUT(L"Width");
    VALIDATE(VHeight);
    LABELOUT(L"Height");
    VALIDATE(VSequence);
    LABELOUT(L"Sequence");
    c += 1;
    VALIDATE(VN);
    LABELOUT(L"Number of iterations");
    VALIDATE(VInitX);
    LABELOUT(L"Initial x value");
    VALIDATE(VAmin);
    LABELOUT(L"a_min");
    VALIDATE(VAmax);
    LABELOUT(L"a_max");
    VALIDATE(VBmin);
    LABELOUT(L"b_min");
    VALIDATE(VBmax);
    LABELOUT(L"b_max");
    VALIDATE(VFunc);
    LABELOUT(L"func");
    VALIDATE(VConst1);
    LABELOUT(L"constance 1");
    VALIDATE(VConst2);
    LABELOUT(L"constance 2");

    SetTextColor(hdc, RGB(0, 0, 0));
}