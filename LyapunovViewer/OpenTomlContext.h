#pragma once
#include <Windows.h>
#include <commdlg.h>
#include "EditContexts.h"

class OpenTomlContext {
	OPENFILENAME ofn = { 0 };
	TCHAR filename[MAX_PATH];
	TCHAR strCustom[256] = TEXT("Before files\0*.*\0\0");
	TCHAR initialCurrentDir[MAX_PATH];

public:
	OpenTomlContext();

	void LoadToml(HWND hWnd, const EditContext& edit);
};