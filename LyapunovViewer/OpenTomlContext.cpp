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
        // filename�Ƀf�[�^���i�[�����
        // TOML��ǂݍ����GetWindowText��Edit�Ɋi�[���Ă���
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
    # plot: matplotlib�ŕ`��, 3d: STL�t�@�C���ɏo��
    mode = "plot"

    # �`��͈�
    width   = 512     # ��
    height  = 512     # �c

    [parameters]
    # param�́A"AABAB"���̕����ƁA41243���̐��l��n����
    param = "AABBAB"
    # �����̏����l
    initX = 0.5
    # �����v�Z��
    N = 10
    # a��b�����ꂼ���蓾��ŏ��l�ƍő�l, a,b in [0, 4]
    amin = 2
    amax = 4
    bmin = 2
    bmax = 4
    # simple, sin2
    func = "simple"
    # �萔
    const = 2.7
    */
}
