#pragma once
#include <windows.h>

void LabelOut(HDC& hdc, const int x, const int y, LPCWSTR label)
{
    TextOut(hdc, x, y, label, lstrlen(label));
}