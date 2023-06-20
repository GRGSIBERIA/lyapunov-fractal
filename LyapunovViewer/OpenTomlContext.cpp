#include "OpenTomlContext.h"
#include <string>
#include <iostream>
#include "toml11/toml.hpp"

OpenTomlContext::OpenTomlContext()
{
    GetCurrentDirectory(MAX_PATH, initialCurrentDir);
}



void OpenTomlContext::LoadToml(HWND hWnd, EditContext& edit)
{
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ZeroMemory(filename, sizeof(TCHAR) * MAX_PATH);

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = TEXT("TOML file {*.toml}\0*.toml\0");
    ofn.lpstrCustomFilter = strCustom;
    ofn.nMaxCustFilter = 256;
    ofn.nFilterIndex = 0;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    ofn.lpstrInitialDir = initialCurrentDir;

    if (GetOpenFileName(&ofn))
    {
        // filenameÇ…ÉfÅ[É^Ç™äiî[Ç≥ÇÍÇÈ
        // TOMLÇì«Ç›çûÇÒÇ≈GetWindowTextÇ≈EditÇ…äiî[ÇµÇƒÇ¢Ç≠
        std::wstring fname(filename);
        std::ifstream ifs(fname, std::ios_base::binary);
        auto parse = toml::parse(ifs);

        auto plot = toml::find(parse, "plottings");
        auto params = toml::find(parse, "parameters");

        try {
            edit.PWidth = toml::find<int>(plot, "width");
            edit.PHeight = toml::find<int>(plot, "height");

            edit.PSequence = toml::find<std::string>(params, "param");
            edit.PInitX = toml::find<float>(params, "initX");
            edit.PN = toml::find<int>(params, "N");
            edit.PAmax = toml::find<float>(params, "amax");
            edit.PAmin = toml::find<float>(params, "amin");
            edit.PBmax = toml::find<float>(params, "bmax");
            edit.PBmin = toml::find<float>(params, "bmin");
            edit.PFunc = toml::find<std::string>(params, "func");
            edit.PConst1 = toml::find<float>(params, "const1");
            edit.PConst2 = toml::find<float>(params, "const2");
        }
        catch (toml::type_error e) {
            auto what = edit.StringToWString(e.what());
            auto loc = edit.StringToWString(e.location().line_str());
            auto mes = what + L":" + loc;

            MessageBox(NULL, mes.c_str(), TEXT("Invalid Toml Error"), MB_OK | MB_ICONERROR);
            return;
        }
        
        edit.convertWString();
        edit.applyEditWindows();
    }
}

void WriteParam(std::wofstream& ofs, HWND& param, const std::wstring& param_name) {
    TCHAR buffer[256];
    GetWindowText(param, buffer, 256);
    std::wstring str = buffer;
    

    if (!param_name.compare(L"param") || !param_name.compare(L"func")) {
        ofs << param_name.c_str() << L" = \"" << str.c_str() << "\"" << std::endl;
    }
    else {
        ofs << param_name.c_str() << L" = " << str.c_str() << std::endl;
    }
}

void OpenTomlContext::SaveToml(HWND hWnd, EditContext& edit)
{
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ZeroMemory(filename, sizeof(TCHAR) * MAX_PATH);

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = TEXT("TOML files {*.toml}\0*.toml\0");
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn)) {
        using namespace std;
        std::wstring fname = filename;
        std::wofstream ofs(fname);
        
        ofs << L"[plottings]" << endl;
        ofs << L"mode = \"plot\"" << endl;

        WriteParam(ofs, edit.Width, L"width");
        WriteParam(ofs, edit.Height, L"height");

        ofs << endl;
        ofs << L"[parameters]" << endl;

        WriteParam(ofs, edit.Sequence, L"param");
        WriteParam(ofs, edit.InitX, L"initX");
        WriteParam(ofs, edit.N, L"N");
        WriteParam(ofs, edit.Amin, L"amin");
        WriteParam(ofs, edit.Amax, L"amax");
        WriteParam(ofs, edit.Bmin, L"bmin");
        WriteParam(ofs, edit.Bmax, L"bmax");
        WriteParam(ofs, edit.Func, L"func");
        WriteParam(ofs, edit.Const1, L"const1");
        WriteParam(ofs, edit.Const2, L"const2");
    }
}
