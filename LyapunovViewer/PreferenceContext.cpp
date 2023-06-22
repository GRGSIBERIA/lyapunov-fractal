#include "PreferenceContext.h"

void PreferenceContext::initializeChoose(HWND& hWnd, CHOOSECOLOR& choose, COLORREF color)
{
	choose.lStructSize = sizeof(CHOOSECOLOR);
	choose.hwndOwner = hWnd;
	choose.rgbResult = color;
	choose.lpCustColors = custColors;
	choose.Flags = CC_FULLOPEN | CC_RGBINIT;
}

void PreferenceContext::initialize(HWND& hWnd, HINSTANCE& hInstance)
{
	_isChaos = CreateWindow(
		TEXT("BUTTON"), TEXT("is Chaos"),
		WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_AUTOCHECKBOX,
		880, 42, 100, 24,
		hWnd, NULL, hInstance, NULL
	);
	
	initializeChoose(hWnd, minchoose, mincolor);
	initializeChoose(hWnd, maxchoose, maxcolor);
}

const bool PreferenceContext::isChaos() const
{
	return BST_CHECKED == SendMessage(_isChaos, BM_GETCHECK, 0, 0);
}
