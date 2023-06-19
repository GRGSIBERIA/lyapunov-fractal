#pragma once
#include <Windows.h>
#include <string>

class EditContext {
    std::wstring SWidth;
    std::wstring SHeight;
    std::wstring SSequence;
    std::wstring SN;
    std::wstring SInitX;
    std::wstring SAmax;
    std::wstring SAmin;
    std::wstring SBmax;
    std::wstring SBmin;
    std::wstring SFunc;
    std::wstring SConst1;
    std::wstring SConst2;

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

    int PWidth;
    int PHeight;
    std::string PSequence;
    int PN;
    float PInitX;
    float PAmax;
    float PAmin;
    float PBmax;
    float PBmin;
    std::string PFunc;
    float PConst1;
    float PConst2;

    void initialize(HWND hWnd, const int x, const int y, const int skip, LPARAM lParam);

    void convertWString();
    void applyEditWindows();

    std::wstring StringToWString(std::string oString) const;
};