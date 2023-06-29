#pragma once
#include <Windows.h>
#include "Resource.h"
#include "EditContexts.h"

void GenerateSpoitButton(HWND hWnd, LPARAM& lParam);
void GenerateLabels(HWND& hWnd, HDC& hdc, const EditContext& edit);