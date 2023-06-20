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

    int PWidth = 0;
    int PHeight = 0;
    std::string PSequence = "";
    int PN = 0;
    float PInitX = 0;
    float PAmax = 0;
    float PAmin = 0;
    float PBmax = 0;
    float PBmin = 0;
    std::string PFunc = "";
    float PConst1 = 0;
    float PConst2 = 0;

    EditContext() {}
    virtual ~EditContext() {}

    EditContext(HWND hWnd, const int x, const int y, const int skip, LPARAM lParam);

    void convertWString();
    void applyEditWindows();

    std::wstring StringToWString(std::string oString) const;
};