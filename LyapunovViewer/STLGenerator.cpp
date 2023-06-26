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

#include <iostream>
#include <fstream>
STLGenerator::STLGenerator(HWND& hWnd, const V2D& lambda, const int width, const int height, const Space& space)
{
	const auto fname = OpenSaveDialog(hWnd, L"STL file {*.stl}\0*.stl\0");
	if (fname.size() <= 0) return;

	std::ofstream ofs(fname, std::ios::binary | std::ios::out);

	// ヘッダの書き込み
	char header[80] = "Provided Lyapunov Fractal Viewer\nAuther: Eiichi Takebuchi (GRGSIBERIA)";
	ofs.write(reinterpret_cast<char*>(&header), 80);

	// 三角形の枚数の書き込み
	unsigned int num_tri = width * height * 12;
	ofs.write(reinterpret_cast<char*>(&num_tri), sizeof(unsigned int));

	/*
	*       P3-------------P4
	*      / |             /|
	*     /  |            / |
	*    P1--+-----------P2 |
	*    |   |           |  |
	*    |  P7-----------+-P8
	*    | /             | /
	*    |/              |/
	*    P5--------------P6
	* 
	* TOP, BOTTOM, FRONT, BACK, LEFT, RIGHT
	* 
	* TOP1 = 1,2,3
	* TOP2 = 2,3,4
	* BOTTOM1 = 5,6,7
	* BOTTOM2 = 6,7,8
	* FRONT1 = 1,2,5
	* FRONT2 = 5,6,2 ....
	*/

	int faces[12][3] = {
		{ 1, 2, 3 },	// T1
		{ 2, 3, 4 },	// T2
		{ 5, 6, 7 },	// B1
		{ 6, 7, 8 },	// B2
		{ 1, 2, 5 },	// F1
		{ 5, 6, 2 },	// F2
		{ 3, 4, 7 },	// BK1
		{ 7, 8, 4 },	// BK2
		{ 1, 3, 5 },	// L1
		{ 5, 7, 3 },	// L2
		{ 2, 4, 6 },	// R1
		{ 6, 8, 4 }		// R2
	};

	float dirs[12][3] = {
		{0, 1, 0},
		{0, 1, 0},
		{0,-1, 0},
		{0,-1, 0},
		{0, 0,-1},
		{0, 0,-1},
		{0, 0, 1},
		{0, 0, 1},
		{-1,0, 0},
		{-1,0, 0},
		{ 1,0, 0},
		{ 1,0, 0}
	};
}
