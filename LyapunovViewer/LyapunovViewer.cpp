// LyapunovViewer.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "LyapunovViewer.h"
#include "toml11/toml.hpp"
#include "Utility.h"
#include <commdlg.h>

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

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
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LYAPUNOVVIEWER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND editTomlPath;
    static HWND 
        editWidth, editHeight, editSequence, editN, editInitX,
        editAmax, editAmin, editBmax, editBmin, 
        editFunc, editConst1, editConst2;

    switch (message)
    {
    case WM_CREATE:
        {
            editTomlPath = CreateWindow(
                TEXT("EDIT"), TEXT(""),
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
                10, 32, 300, 24, hWnd, (HMENU)1,
                ((LPCREATESTRUCT)(lParam))->hInstance, NULL
            );
            
            CreateWindow(
                TEXT("BUTTON"), TEXT("..."),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                320, 32, 32, 24,
                hWnd, (HMENU)BUTTON_ID_OPEN_FILE_DIALOG, ((LPCREATESTRUCT)(lParam))->hInstance, NULL
            );

            /*
            * WM_COMMANDで文字列を取得可能
            * PSTR pStr[2048];
            * GetWindowText(editTomlPath, pStr, 2048);
            */
            
            int c = 0;
            CreateEdit(hWnd, 200, 80 + 32 * c++, 100, 25, lParam, editWidth);
            CreateEdit(hWnd, 200, 80 + 32 * c++, 100, 25, lParam, editHeight);
            CreateEdit(hWnd, 200, 80 + 32 * c++, 100, 25, lParam, editSequence);
            CreateEdit(hWnd, 200, 80 + 32 * c++, 100, 25, lParam, editN);
            CreateEdit(hWnd, 200, 80 + 32 * c++, 100, 25, lParam, editInitX);
            CreateEdit(hWnd, 200, 80 + 32 * c++, 100, 25, lParam, editAmin);
            CreateEdit(hWnd, 200, 80 + 32 * c++, 100, 25, lParam, editAmax);
            CreateEdit(hWnd, 200, 80 + 32 * c++, 100, 25, lParam, editBmin);
            CreateEdit(hWnd, 200, 80 + 32 * c++, 100, 25, lParam, editBmax);
            CreateEdit(hWnd, 200, 80 + 32 * c++, 100, 25, lParam, editFunc);
            CreateEdit(hWnd, 200, 80 + 32 * c++, 100, 25, lParam, editConst1);
            CreateEdit(hWnd, 200, 80 + 32 * c++, 100, 25, lParam, editConst2);
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
                static OPENFILENAME ofn = { 0 };
                TCHAR filename[MAX_PATH], strCustom[256] = TEXT("Before files\0*.*\0\0");;
                
                ZeroMemory(&ofn, sizeof(OPENFILENAME));

                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFilter = TEXT("Text files {*.txt}\0*.txt\0")
                    TEXT("HTML files {*.htm}\0*.htm;*.html\0")
                    TEXT("All files {*.*}\0*.*\0\0");
                ofn.lpstrCustomFilter = strCustom;
                ofn.nMaxCustFilter = 256;
                ofn.nFilterIndex = 0;
                ofn.lpstrFile = filename;
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_FILEMUSTEXIST;

                break;
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

            int c = 0;
            LabelOut(hdc, 10, 10, L"TOML Setting file path");

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
            
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
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
