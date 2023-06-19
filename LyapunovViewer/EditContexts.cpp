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

// https://www.wabiapp.com/WabiSampleSource/windows/string_to_wstring.html
std::wstring EditContext::StringToWString(std::string oString) const
{
    // SJIS → wstring
    int iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, (wchar_t*)NULL, 0);

    // バッファの取得
    wchar_t* cpUCS2 = new wchar_t[iBufferSize];

    // SJIS → wstring
    MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, cpUCS2, iBufferSize);

    // stringの生成
    std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

    // バッファの破棄
    delete[] cpUCS2;

    // 変換結果を返す
    return(oRet);
}

void EditContext::convertWString()
{
    SWidth = std::to_wstring(PWidth);
    SHeight = std::to_wstring(PHeight);
    SSequence = StringToWString(PSequence);
    SN = std::to_wstring(PN);
    SInitX = std::to_wstring(PInitX);
    SAmax = std::to_wstring(PAmax);
    SAmin = std::to_wstring(PAmin);
    SBmax = std::to_wstring(PBmax);
    SBmin = std::to_wstring(PBmin);
    SFunc = StringToWString(PFunc);
    SConst1 = std::to_wstring(PConst1);
    SConst2 = std::to_wstring(PConst2);
}

void EditContext::applyEditWindows()
{
#define SWTM(PARAM) SetWindowText(PARAM, S##PARAM.c_str())
    SWTM(Width);
    SWTM(Height);
    SWTM(Sequence);
    SWTM(N);
    SWTM(InitX);
    SWTM(Amax);
    SWTM(Amin);
    SWTM(Bmax);
    SWTM(Bmin);
    SWTM(Func);
    SWTM(Const1);
    SWTM(Const2);
}
