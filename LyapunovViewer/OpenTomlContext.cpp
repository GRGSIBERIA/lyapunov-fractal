#include "OpenTomlContext.h"

OpenTomlContext::OpenTomlContext()
{
	
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

    if (GetOpenFileName(&ofn))
    {
        // filenameÇ…ÉfÅ[É^Ç™äiî[Ç≥ÇÍÇÈ
        // TOMLÇì«Ç›çûÇÒÇ≈GetWindowTextÇ≈EditÇ…äiî[ÇµÇƒÇ¢Ç≠
        std::wstring fname(filename);
        std::ifstream ifs(fname, std::ios_base::binary);
        auto parse = toml::parse(ifs);

        
    }
}
