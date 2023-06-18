#pragma once
#include <windows.h>

void LabelOut(HDC& hdc, const int x, const int y, LPCWSTR label)
{
    TextOut(hdc, x, y, label, lstrlen(label));
    MoveToEx(hdc, x, y + 24, NULL);
    LineTo(hdc, 300, y + 24);
}

HWND CreateEdit(HWND hWnd, const int x, const int y, const int w, const int h, LPARAM& lParam)
{
    auto ret = CreateWindow(
        TEXT("EDIT"), TEXT(""),
        WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
        x, y, w, h, hWnd, (HMENU)1,
        ((LPCREATESTRUCT)(lParam))->hInstance, NULL
    );
    return ret;
}