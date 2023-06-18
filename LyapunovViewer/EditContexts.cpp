#include "EditContexts.h"

void CreateEdit(HWND hWnd, const int x, const int y, const int w, const int h, LPARAM& lParam, HWND& edit)
{
    edit = CreateWindow(
        TEXT("EDIT"), TEXT(""),
        WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
        x, y, w, h, hWnd, (HMENU)1,
        ((LPCREATESTRUCT)(lParam))->hInstance, NULL
    );
}

void EditContext::initialize(HWND hWnd, const int x, const int y, const int skip, LPARAM lParam)
{
    int c = 0;
    CreateEdit(hWnd, x, y + skip * c++, 100, 25, lParam, Width);
    CreateEdit(hWnd, x, y + skip * c++, 100, 25, lParam, Height);
    CreateEdit(hWnd, x, y + skip * c++, 100, 25, lParam, Sequence);
    CreateEdit(hWnd, x, y + skip * c++, 100, 25, lParam, N);
    CreateEdit(hWnd, x, y + skip * c++, 100, 25, lParam, InitX);
    CreateEdit(hWnd, x, y + skip * c++, 100, 25, lParam, Amin);
    CreateEdit(hWnd, x, y + skip * c++, 100, 25, lParam, Amax);
    CreateEdit(hWnd, x, y + skip * c++, 100, 25, lParam, Bmin);
    CreateEdit(hWnd, x, y + skip * c++, 100, 25, lParam, Bmax);
    CreateEdit(hWnd, x, y + skip * c++, 100, 25, lParam, Func);
    CreateEdit(hWnd, x, y + skip * c++, 100, 25, lParam, Const1);
    CreateEdit(hWnd, x, y + skip * c++, 100, 25, lParam, Const2);
}
