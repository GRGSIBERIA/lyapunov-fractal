﻿// LyapunovViewer.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "LyapunovViewer.h"
#include "toml11/toml.hpp"
#include <commdlg.h>
#include <iostream>

#define MAX_LOADSTRING 100
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 680

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名
#define MDI_CHILD TEXT("MDICHILD")
// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LYAPUNOVVIEWER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LYAPUNOVVIEWER));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LYAPUNOVVIEWER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LYAPUNOVVIEWER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    // RGB(212, 207, 201)

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd) return FALSE;
   
   RECT rect;
   GetClientRect(hWnd, &rect);

   SetWindowPos(hWnd, NULL, rect.left, rect.top, WINDOW_WIDTH, WINDOW_HEIGHT, (SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE));
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

#include "EditContexts.h"
#include "OpenTomlContext.h"
#include "GeneratorContext.h"
#include "ImageContext.h"
#include "PreferenceContext.h"
#include "PlotterContext.h"
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static EditContext edit;
    static OpenTomlContext open;
    static ImageContext image;
    static PreferenceContext prefer;
    static PlotterContext plotter;
    
    static TCHAR currentDir[MAX_PATH];
    static HWND hClient;
    static bool isGenerated = false;

    switch (message)
    {
    case WM_CREATE:
    {
        GetCurrentDirectory(MAX_PATH, currentDir);

        CreateWindow(
            TEXT("BUTTON"), TEXT("LOAD SETTING FILE"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            40, 42, 260, 24,
            hWnd, (HMENU)BUTTON_ID_OPEN_FILE_DIALOG, ((LPCREATESTRUCT)(lParam))->hInstance, NULL
        );

        CreateWindow(
            TEXT("BUTTON"), TEXT("RUN LYAPUNOV FRACTAL"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            40, 512, 260, 24,
            hWnd, (HMENU)BUTTON_ID_RUN_LYANUNOV, ((LPCREATESTRUCT)(lParam))->hInstance, NULL
        );
            
        CreateWindow(
            TEXT("BUTTON"), TEXT("SAVE CURRENT SETTINGS"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            40, 512 + 32, 260, 24,
            hWnd, (HMENU)BUTTON_ID_SAVE_FILE_DIALOG, ((LPCREATESTRUCT)(lParam))->hInstance, NULL
        );

        CreateWindow(
            TEXT("BUTTON"), TEXT("OPEN 3D PLOTTING WINDOW"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            40, 512 + 64, 260, 24,
            hWnd, (HMENU)BUTTON_ID_OPEN_3D_WINDOW, ((LPCREATESTRUCT)(lParam))->hInstance, NULL
        );

        GenerateSpoitButton(hWnd, lParam);

        /*
        * WM_COMMANDで文字列を取得可能
        * PSTR pStr[2048];
        * GetWindowText(editTomlPath, pStr, 2048);
        */
            
        edit.initialize(hWnd, 200, 80, 32, lParam);
        image.initialize(hWnd, 512, 512);
        prefer.initialize(hWnd, ((LPCREATESTRUCT)(lParam))->hInstance);

        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 選択されたメニューの解析:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;

        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;

        case BUTTON_ID_OPEN_FILE_DIALOG:
        {
            open.LoadToml(hWnd, edit);
            break;
        }
        case BUTTON_ID_SAVE_FILE_DIALOG:
        {
            open.SaveToml(hWnd, edit);
            break;
        }
        case BUTTON_ID_RUN_LYANUNOV:
        {
            if (!edit.validateValues(hWnd)) {
                InvalidateRect(hWnd, NULL, FALSE);
                break;
            }
            if (!edit.applyValues()) break;

            image.initialize(hWnd, edit.PWidth, edit.PHeight);
            image.setIsChaos(prefer.isChaos());
            image.generate(hWnd, edit);

            isGenerated = true;

            SendMessage(hWnd, WM_PAINT, wParam, lParam);
            break;
        }
        case BUTTON_ID_SET_MIN_COLOR:
        {
            prefer.chooseColor(hWnd, PreferenceContext::Choose::MIN);
            image.setMinColor(prefer.getMinColor());
            break;
        }
        case BUTTON_ID_SET_MAX_COLOR:
        {
            prefer.chooseColor(hWnd, PreferenceContext::Choose::MAX);
            image.setMaxColor(prefer.getMaxColor());
            break;
        }
        case BUTTON_ID_SPOIT_AMAX:
        {
            edit.offTriggers();
            edit.amaxTrig = !edit.amaxTrig;
            break;
        }
        case BUTTON_ID_SPOIT_AMIN:
        {
            edit.offTriggers();
            edit.aminTrig = !edit.aminTrig;
            break;
        }
        case BUTTON_ID_SPOIT_BMAX:
        {
            edit.offTriggers();
            edit.bmaxTrig = !edit.bmaxTrig;
            break;
        }
        case BUTTON_ID_SPOIT_BMIN:
        {
            edit.offTriggers();
            edit.bminTrig = !edit.bminTrig;
            break;
        }
        case BUTTON_ID_SPOIT_TL:
        {
            edit.offTriggers();
            edit.aminTrig = !edit.aminTrig;
            edit.bminTrig = !edit.bminTrig;
            break;
        }
        case BUTTON_ID_SPOIT_BR:
        {
            edit.offTriggers();
            edit.amaxTrig = !edit.amaxTrig;
            edit.bmaxTrig = !edit.bmaxTrig;
            break;
        }
        case BUTTON_ID_SPOIT_CANCEL:
        {
            edit.offTriggers();
            break;
        }
        case BUTTON_ID_SPOIT_HOME:
        {
            edit.PAmax = 4.f;
            edit.PAmin = 0.f;
            edit.PBmax = 4.f;
            edit.PBmin = 0.f;
            edit.convertWString();
            edit.applyEditWindows();
            break;
        }
        case BUTTON_ID_OPEN_3D_WINDOW:
        {
            if (!isGenerated) {
                MessageBox(NULL, TEXT("Fractal was not generated"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
                break;
            }
            plotter.initialize(hInst, hWnd, image);
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    }
    case WM_LBUTTONDOWN:
    {
        // トリガーがオンになっていたら、AB座標の更新を行う
        if (edit.enableTriggers()) {
            //edit.setMousePos(lParam); // WM_MOUSEMOVEとでマウス座標がずれる可能性がある
            const auto p = edit.getABPos();

            if (edit.amaxTrig) edit.PAmax = p.x;
            if (edit.aminTrig) edit.PAmin = p.x;
            if (edit.bmaxTrig) edit.PBmax = p.y;
            if (edit.bminTrig) edit.PBmin = p.y;

            edit.convertWString();
            edit.applyEditWindows();
            edit.offTriggers();
        }
        
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: HDC を使用する描画コードをここに追加してください...

        SetBkColor(hdc, RGB(192,192,192));

        GenerateLabels(hWnd, hdc, edit);
        TextOut(hdc, 320, 10, L"Spoit", lstrlen(L"Spoit"));
        
        // 記録の有無を判定して表示する
        if (!edit.enableTriggers()) {
            SelectObject(hdc, CreateSolidBrush(RGB(0, 255, 0)));
        }
        else {
            SelectObject(hdc, CreateSolidBrush(RGB(255, 0, 0)));
        }
        Ellipse(hdc, 900, 10, 900 + 20, 10 + 20);
        DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
            
        image.draw(hdc, hWnd);
        prefer.draw(hdc);
        edit.draw(hdc);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
    {
        image.destroy();
        PostQuitMessage(0);
        break;
    }
    case WM_MOUSEMOVE:
    {
        edit.setMousePos(lParam);
        RECT r;
        r.left = 880;
        r.top = 10 + 32 * 4;
        r.right = 1080;
        r.bottom = 40 + 32 * 4;
        
        InvalidateRect(hWnd, &r, FALSE);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
