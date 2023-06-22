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
	int c = 0;

	_isChaos = CreateWindow(
		TEXT("BUTTON"), TEXT("is Chaos"),
		WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_AUTOCHECKBOX,
		880, 42 + 32 * c++, 100, 24,
		hWnd, NULL, hInstance, NULL
	);
	
	initializeChoose(hWnd, minchoose, mincolor);
	initializeChoose(hWnd, maxchoose, maxcolor);

	CreateWindow(
		TEXT("BUTTON"), TEXT("SET MIN COLOR"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		880, 42 + 32 * c++, 128, 24,
		hWnd, (HMENU)BUTTON_ID_SET_MIN_COLOR, hInstance, NULL
	);
	CreateWindow(
		TEXT("BUTTON"), TEXT("SET MAX COLOR"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		880, 42 + 32 * c++, 128, 24,
		hWnd, (HMENU)BUTTON_ID_SET_MAX_COLOR, hInstance, NULL
	);
}

void PreferenceContext::draw(HDC& hdc)
{
	SelectObject(hdc, CreateSolidBrush(minchoose.rgbResult));
	PatBlt(hdc, 880 + 128 + 8, 42 + 32, 24, 24, PATCOPY);
	DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));

	SelectObject(hdc, CreateSolidBrush(maxchoose.rgbResult));
	PatBlt(hdc, 880 + 128 + 8, 42 + 32 + 32, 24, 24, PATCOPY);
	DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
}

const bool PreferenceContext::isChaos() const
{
	return BST_CHECKED == SendMessage(_isChaos, BM_GETCHECK, 0, 0);
}

const bool PreferenceContext::chooseColor(HWND& hWnd, const Choose& chooser)
{
	switch (chooser) {
	case Choose::MIN:
		ChooseColor(&minchoose);
		break;
	case Choose::MAX:
		ChooseColor(&maxchoose);
		break;
	default:
		return false;
	}
	InvalidateRect(hWnd, NULL, TRUE);
	return true;
}
