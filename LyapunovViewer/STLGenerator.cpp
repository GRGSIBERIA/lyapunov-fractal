#include "STLGenerator.h"

const std::wstring OpenSaveDialog(HWND& hWnd, const std::wstring& filter) {
	OPENFILENAME ofn = { 0 };
	TCHAR initialCurrentDir[MAX_PATH];
	TCHAR filename[MAX_PATH] = { '\0' };

	ZeroMemory(filename, sizeof(TCHAR) * MAX_PATH);

	GetCurrentDirectory(MAX_PATH, initialCurrentDir);

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = filter.c_str();
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = initialCurrentDir;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&ofn))
		return filename;

	return L"";
}

STLGenerator::STLGenerator(HWND& hWnd, const V2D& lambda)
{
	const auto fname = OpenSaveDialog(hWnd, L"STL file {*.stl}\0*.stl\0");
	if (fname.size() <= 0) return;


}
