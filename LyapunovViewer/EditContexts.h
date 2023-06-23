#pragma once
#include <Windows.h>
#include <string>

class EditContext {
    std::wstring SWidth = L"512";
    std::wstring SHeight = L"512";
    std::wstring SSequence = L"AABB";
    std::wstring SN = L"10";
    std::wstring SInitX = L"0.5";
    std::wstring SAmax = L"4.0";
    std::wstring SAmin = L"0.0";
    std::wstring SBmax = L"4.0";
    std::wstring SBmin = L"0.0";
    std::wstring SFunc = L"simple";
    std::wstring SConst1 = L"0.0";
    std::wstring SConst2 = L"0.0";

    int mx = 0;
    int my = 0;

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

    int PWidth = 512;
    int PHeight = 512;
    std::string PSequence = "AABB";
    int PN = 10;
    float PInitX = 0.5;
    float PAmax = 0;
    float PAmin = 4;
    float PBmax = 0;
    float PBmin = 4;
    std::string PFunc = "simple";
    float PConst1 = 0;
    float PConst2 = 0;

    EditContext() {}
    virtual ~EditContext() {}

    void initialize(HWND hWnd, const int x, const int y, const int skip, LPARAM lParam);
    void draw(HDC& hdc);

    void convertWString();
    void applyEditWindows();
    void applyValues();

    std::wstring StringToWString(std::string oString) const;
    void setMousePos(LPARAM& lParam) { mx = LOWORD(lParam); my = HIWORD(lParam); }

    const POINTFLOAT getABPos() const;

    bool amaxTrig = false;
    bool aminTrig = false;
    bool bmaxTrig = false;
    bool bminTrig = false;
};