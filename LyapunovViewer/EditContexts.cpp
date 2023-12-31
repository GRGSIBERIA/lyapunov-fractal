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

void EditContext::offTriggers()
{
    amaxTrig = false;
    aminTrig = false;
    bmaxTrig = false;
    bminTrig = false;
}

#include <format>
void EditContext::draw(HDC& hdc)
{
    const auto p = getABPos();

    // 初回起動時はなぜか符号が反転している
    const auto str = std::format(L"A={:.6f}, B={:.6f}", p.x, p.y);
    TextOut(hdc, 880, 10 + 32 * 4, str.c_str(), lstrlen(str.c_str()));
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

const bool EditContext::applyValues()
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

    return true;
}

const std::wstring GetName(const HWND& hWnd, const std::wstring& name) {
    const size_t N = 256;
    TCHAR buf[N];
    GetWindowText(hWnd, buf, N);
    return std::wstring(buf);
}

#define SHOWMSGBOX MessageBox(NULL, str.c_str(), TEXT("ERROR"), MB_OK | MB_ICONERROR)

const bool ValidateDecodeStoI(const HWND& hWnd, const std::wstring& name) {
    const size_t N = 256;
    TCHAR buf[N];
    int itest;

    GetWindowText(hWnd, buf, N);
    try {
        itest = std::stoi(buf);
    }
    catch (...) {
        std::wstring str;
        std::wstring bufStr(buf);

        if (bufStr.size() <= 0)
            str = std::format(L"Empty {}", name);
        else
            str = std::format(L"Can't decode text box\n {} = {}", name, buf);

        SHOWMSGBOX;
        return false;
    }

    return true;
}

const bool ValidateDecodeStoF(const HWND& hWnd, const std::wstring& name) {
    const size_t size = 256;
    TCHAR buf[size];
    float ftest;

    GetWindowText(hWnd, buf, size);
    try {
        ftest = std::stof(buf);
    }
    catch (...) {
        std::wstring str;

        if (str.size() <= 0)
            str = std::format(L"Empty {}", name);
        else
            str = std::format(L"Can't decode text box\n {} = {}", name, buf);

        SHOWMSGBOX;
        return false;
    }

    return true;
}

template <class T>
const T GetValue(const HWND& hWnd) {
    const size_t size = 256;
    TCHAR buf[size];
    T test;

    GetWindowText(hWnd, buf, size);
    std::wstringstream ws;
    ws << std::wstring(buf);
    ws >> test;

    return test;
}

template <class T>
const bool ValidateRange(const HWND& hWnd, const std::wstring& name, const T vmin, const T vmax) {
    T test = GetValue<T>(hWnd);

    try {
        if (!(vmin <= test && test <= vmax)) throw std::exception();
    }
    catch (...) {
        const auto str = std::format(L"Falls within the range\n{} <= {} <= {}\n{} = {}", vmin, name, vmax, name, test);
        SHOWMSGBOX;
        return false;
    }

    return true;
}

template <class T>
const bool ValidateUnderCover(const HWND& hWnd, const std::wstring& name) {
    T test = GetValue<T>(hWnd);
    
    try {
        if (!((T)0 < test)) throw std::exception();
    }
    catch (...) {
        const auto str = std::format(L"{} less than 1\n{} = {}", name, name, test);
        SHOWMSGBOX;
        return false;
    }

    return true;
}

template <class T>
const bool ValidateModulo8(const HWND& hWnd, const std::wstring& name) {
    T test = GetValue<T>(hWnd);
    
    try {
        if (!(test % 8 == 0)) throw std::exception();
    }
    catch (...) {
        const auto str = std::format(L"{} is not modulo 0 when divided by 8\n{} = {} mod 8 = ", name, name, test, test % 4);
        SHOWMSGBOX;
        return false;
    }

    return true;
}

const bool ValidateSequence(const HWND& hWnd) {
    std::wstring seq = GetValue<std::wstring>(hWnd);

    if (seq.size() == 0) {
        const auto str = std::format(L"Sequence is empty");
        SHOWMSGBOX;
        return false;
    }

    if ((L'0' <= seq[0] && seq[0] <= L'9')) {
        const auto str = std::format(L"Sequence's first character is number\nSequence = {}", seq);
        SHOWMSGBOX;
        return false;
    }

    for (int i = 0; i < seq.size(); ++i) {
        if (seq[i] == L'A' || seq[i] == L'B') continue;
        if (L'0' <= seq[i] && seq[i] <= L'9') continue;

        const auto str = std::format(L"Sequence can has A,B,0-9\nSequence = {}", seq);
        SHOWMSGBOX;
        return false;
    }

    return true;
}

const bool ValidateEmpty(const HWND& hWnd, const std::wstring& name) {
    const auto test = GetName(hWnd, name);

    if (test.size() <= 0) {
        const auto str = std::format(L"Empty textbox: {}", name);
        SHOWMSGBOX;
        return false;
    }
    return true;
}

const bool ValidateFunc(const HWND& hWnd, const std::wstring& name) {
    const auto test = GetName(hWnd, name);

    if (test == L"simple") return true;
    if (test == L"cyclic") return true;

    const auto str = std::format(L"Invalid Function Name\nFunc = {}", test);
    SHOWMSGBOX;
    return false;
}

const bool EditContext::validateValues(HWND& hWnd)
{
    bool valid = true;

    VWidth = true;
    VHeight = true;
    VSequence = true;
    VN = true;
    VInitX = true;
    VAmax = true;
    VAmin = true;
    VBmax = true;
    VBmin = true;
    VFunc = true;
    VConst1 = true;
    VConst2 = true;

    VWidth &= ValidateDecodeStoI(Width, L"Width");
    VHeight &= ValidateDecodeStoI(Height, L"Height");
    VHeight &= ValidateDecodeStoI(N, L"Number of iterations");
    
    VSequence &= ValidateEmpty(Sequence, L"Sequence");
    VFunc &= ValidateEmpty(Func, L"Func");
    VFunc &= ValidateFunc(Func, L"Func");
    
    VAmax &= ValidateDecodeStoF(Amax, L"Amax");
    VAmin &= ValidateDecodeStoF(Amin, L"Amin");
    VBmax &= ValidateDecodeStoF(Bmax, L"Bmax");
    VBmin &= ValidateDecodeStoF(Bmin, L"Bmin");
    VConst1 &= ValidateDecodeStoF(Const1, L"Const1");
    VConst2 &= ValidateDecodeStoF(Const2, L"Const2");
    VInitX &= ValidateDecodeStoF(InitX, L"Initial x value");
    
    VAmax &= ValidateRange<float>(Amax, L"Amax", 0.f, 4.f);
    VAmin &= ValidateRange<float>(Amin, L"Amin", 0.f, 4.f);
    VBmax &= ValidateRange<float>(Bmax, L"Bmax", 0.f, 4.f);
    VBmin &= ValidateRange<float>(Bmin, L"Bmin", 0.f, 4.f);
    
    VWidth &= ValidateUnderCover<int>(Width, L"Width");
    VHeight &= ValidateUnderCover<int>(Height, L"Height");
    VN &= ValidateUnderCover<int>(N, L"Number of iterations");
    
    VWidth &= ValidateModulo8<int>(Width, L"Width");
    VHeight &= ValidateModulo8<int>(Height, L"Height");

    VSequence &= ValidateSequence(Sequence);

    valid = valid && VWidth;
    valid = valid && VHeight;
    valid = valid && VSequence;
    valid = valid && VFunc;
    valid = valid && VAmax;
    valid = valid && VAmin;
    valid = valid && VBmax;
    valid = valid && VBmin;
    valid = valid && VConst1;
    valid = valid && VConst2;
    valid = valid && VInitX;
    valid = valid && VN;

    return valid;
}