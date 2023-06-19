#include "OpenTomlContext.h"

OpenTomlContext::OpenTomlContext()
{
    GetCurrentDirectory(MAX_PATH, initialCurrentDir);
}

#include <string>
#include <iostream>
#include "toml11/toml.hpp"
void OpenTomlContext::LoadToml(HWND hWnd, const EditContext& edit)
{
    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = TEXT("TOML files {*.toml}\0*.toml\0");
    ofn.lpstrCustomFilter = strCustom;
    ofn.nMaxCustFilter = 256;
    ofn.nFilterIndex = 0;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    ofn.lpstrInitialDir = initialCurrentDir;

    if (GetOpenFileName(&ofn))
    {
        // filenameにデータが格納される
        // TOMLを読み込んでGetWindowTextでEditに格納していく
        std::wstring fname(filename);
        std::ifstream ifs(fname, std::ios_base::binary);
        auto parse = toml::parse(ifs);

        auto plot = toml::find(parse, "plottings");
        auto param = toml::find(parse, "parameters");

        auto mode = toml::find<std::string>(plot, "mode");
        auto width = toml::find<int>(plot, "width");
        auto height = toml::find<int>(plot, "height");
    }
    /*
    [plottings]
    # plot: matplotlibで描画, 3d: STLファイルに出力
    mode = "plot"

    # 描画範囲
    width   = 512     # 横
    height  = 512     # 縦

    [parameters]
    # paramは、"AABAB"等の文字と、41243等の数値を渡せる
    param = "AABBAB"
    # 成長の初期値
    initX = 0.5
    # 反復計算回数
    N = 10
    # aとbがそれぞれ取り得る最小値と最大値, a,b in [0, 4]
    amin = 2
    amax = 4
    bmin = 2
    bmax = 4
    # simple, sin2
    func = "simple"
    # 定数
    const = 2.7
    */
}
