#pragma once
#include <Windows.h>

class EditContext {
public:
    HWND Width = NULL;
    HWND Height = NULL;
    HWND Sequence = NULL;
    HWND N = NULL;
    HWND InitX = NULL;
    HWND Amax = NULL;
    HWND Amin = NULL;
    HWND Bmax = NULL;
    HWND Bmin = NULL;
    HWND Func = NULL;
    HWND Const1 = NULL;
    HWND Const2 = NULL;

    void initialize(HWND hWnd, const int x, const int y, const int skip, LPARAM lParam);
};