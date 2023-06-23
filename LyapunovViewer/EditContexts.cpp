#include "EditContexts.h"
#include <string>

void CreateEdit(HWND hWnd, const int x, const int y, const int w, const int h, LPARAM& lParam, HWND& edit, std::wstring& value)
{
    edit = CreateWindow(
        TEXT("EDIT"), value.c_str(),
        WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
        x, y, w, h, hWnd, (HMENU)1,
        ((LPCREATESTRUCT)(lParam))->hInstance, NULL
    );
}

void EditContext::initialize(HWND hWnd, const int x, const int y, const int skip, LPARAM lParam)
{
#define INITEDIT(PARAM) CreateEdit(hWnd, x, y + skip * c++, 100, 25, lParam, PARAM, S##PARAM)
    int c = 0;
    INITEDIT(Width);
    INITEDIT(Height);
    //INITEDIT(Sequence);
    // 幅が異なるので、マクロは使えない
    Sequence = CreateWindow(
        TEXT("EDIT"), SSequence.c_str(),
        WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
        40, y + skip * (c + 1), 260, 25, hWnd, (HMENU)1,
        ((LPCREATESTRUCT)(lParam))->hInstance, NULL);
    c += 2;
    INITEDIT(N);
    INITEDIT(InitX);
    INITEDIT(Amin);
    INITEDIT(Amax);
    INITEDIT(Bmin);
    INITEDIT(Bmax);
    INITEDIT(Func);
    INITEDIT(Const1);
    INITEDIT(Const2);
}

const POINTFLOAT EditContext::getABPos() const
{
    POINTFLOAT p;

    const int curW = 512;
    const int curH = 512;
    const int cx = 344;
    const int cy = 42;

    float x = (float)mx - (float)cx;
    float y = (float)my - (float)cy;

    x = x >= 0.f ? x : 0.f;
    x = x <= (float)curW ? x : (float)curW;
    y = y >= 0.f ? y : 0.f;
    y = y <= (float)curH ? y : (float)curH;

    float dw = 1.f / (float)curW;
    float dh = 1.f / (float)curH;

    float rx = x / (float)curW;
    float ry = y / (float)curH;

    p.x = (PAmax - PAmin) * rx + PAmin;
    p.y = (PBmax - PBmin) * ry + PBmin;

    return p;
}

#include <format>
void EditContext::draw(HDC& hdc)
{
    const auto p = getABPos();

    // 初回起動時はなぜか符号が反転している
    const auto str = std::format(L"A={:.5f}, B={:.5f}", p.x, p.y);
    TextOut(hdc, 750, 10, str.c_str(), lstrlen(str.c_str()));
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

#include <locale>
#include <codecvt>
#include <Windows.h>
std::string ConvertWstringToUTF8(const std::wstring& src)
{
    const size_t N = 1024;
    char local[N] = { 0 };
    WideCharToMultiByte(
        CP_ACP,
        0,
        src.c_str(),
        -1,
        local,
        N + 1,
        NULL,
        NULL);
    return std::string(local);
}

void EditContext::applyValues()
{
    TCHAR buf[256];
#define GWTi(PARAM) { GetWindowText(PARAM, buf, 256); P##PARAM = std::stoi(buf); }
#define GWTs(PARAM) { GetWindowText(PARAM, buf, 256); P##PARAM = ConvertWstringToUTF8(std::wstring(buf)); }
#define GWTf(PARAM) { GetWindowText(PARAM, buf, 256); P##PARAM = std::stof(buf); }
    GWTi(Width);
    GWTi(Height);
    GWTs(Sequence);
    GWTi(N);
    GWTf(InitX);
    GWTf(Amax);
    GWTf(Amin);
    GWTf(Bmax);
    GWTf(Bmin);
    GWTs(Func);
    GWTf(Const1);
    GWTf(Const2);
}
