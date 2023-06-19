// LyapunovViewer.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "LyapunovViewer.h"
#include "toml11/toml.hpp"
#include "Utility.h"
#include <commdlg.h>
#include <iostream>

#define MAX_LOADSTRING 100

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
    RegisterClassExW(&wcex);

    wcex.lpfnWndProc = DefMDIChildProc;
    wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcex.lpszClassName = MDI_CHILD;

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

   SetWindowPos(hWnd, NULL, rect.left, rect.top, 800, 640, (SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE));
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
#include "EditContexts.h"
#include "OpenTomlContext.h"
#include "LyapunovWindow.h"
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static EditContext edit;
    static OpenTomlContext open;
    static LyapunovWindow lyapunov;
    static TCHAR currentDir[MAX_PATH];
    static HWND hClient;

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
                40, 512 - 32, 260, 24,
                hWnd, (HMENU)BUTTON_ID_RUN_LYANUNOV, ((LPCREATESTRUCT)(lParam))->hInstance, NULL
            );
            
            CreateWindow(
                TEXT("BUTTON"), TEXT("SAVE CURRENT SETTINGS"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                40, 512, 260, 24,
                hWnd, (HMENU)BUTTON_ID_SAVE_FILE_DIALOG, ((LPCREATESTRUCT)(lParam))->hInstance, NULL
            );

            // MDIクライアントウィンドウを作る
            // まずはフレームの作成
            CLIENTCREATESTRUCT ccs;
            MDICREATESTRUCT mdic;

            ccs.hWindowMenu = NULL;
            ccs.idFirstChild = 50000;
            hClient = CreateWindow(TEXT("MDICLIENT"), NULL,
                WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
                320, 0, 480, 640, hWnd, (HMENU)1, hInst, &ccs
            );
            // 小窓の作成
            CreateMDIWindow(TEXT("FRAMEWINDOW"), NULL,
                WS_HSCROLL | WS_VSCROLL,
                CW_USEDEFAULT, CW_USEDEFAULT,
                CW_USEDEFAULT, CW_USEDEFAULT,
                hClient, hInst, 0
            );

            mdic.szClass = MDI_CHILD;
            mdic.szTitle = szTitle;
            mdic.x = mdic.y = mdic.cx = mdic.cy = CW_USEDEFAULT;
            mdic.style = mdic.lParam = 0;
            SendMessage(hClient, WM_MDICREATE, 0, (LPARAM)&mdic);

            /*
            * WM_COMMANDで文字列を取得可能
            * PSTR pStr[2048];
            * GetWindowText(editTomlPath, pStr, 2048);
            */
            
            edit.initialize(hWnd, 200, 80, 32, lParam);
        }
        break;
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
                break;
            }
            case BUTTON_ID_RUN_LYANUNOV:
            {
                lyapunov.initWindow(hInst, hWnd);
                break;
            }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }

        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...

            SetBkColor(hdc, RGB(192,192,192));

            int c = 0;
            LabelOut(hdc, 40, 80 + 32 * c++, L"Width");
            LabelOut(hdc, 40, 80 + 32 * c++, L"Height");
            LabelOut(hdc, 40, 80 + 32 * c++, L"Sequence");
            LabelOut(hdc, 40, 80 + 32 * c++, L"Number of iterations");
            LabelOut(hdc, 40, 80 + 32 * c++, L"Initial x value");
            LabelOut(hdc, 40, 80 + 32 * c++, L"a_min");
            LabelOut(hdc, 40, 80 + 32 * c++, L"a_max");
            LabelOut(hdc, 40, 80 + 32 * c++, L"b_min");
            LabelOut(hdc, 40, 80 + 32 * c++, L"b_max");
            LabelOut(hdc, 40, 80 + 32 * c++, L"func");
            LabelOut(hdc, 40, 80 + 32 * c++, L"constance 1");
            LabelOut(hdc, 40, 80 + 32 * c++, L"constance 2");
            int hoge = 80 + 32 * c++ + 16;
            
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefFrameProc(hWnd, hClient, message, wParam, lParam);
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
