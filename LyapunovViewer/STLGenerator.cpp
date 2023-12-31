#include "STLGenerator.h"

const std::wstring OpenSaveDialog(HWND& hWnd, const std::wstring& filter) {
	OPENFILENAME ofn = { 0 };
	TCHAR initialCurrentDir[MAX_PATH];
	TCHAR filename[MAX_PATH] = TEXT("output.stl");
	TCHAR strCustom[256] = TEXT("Before files\0*.*\0\0");

	ZeroMemory(filename, sizeof(TCHAR) * MAX_PATH);

	GetCurrentDirectory(MAX_PATH, initialCurrentDir);

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = filter.c_str();
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = initialCurrentDir;
	ofn.lpstrCustomFilter = strCustom;
	ofn.nMaxCustFilter = 256;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&ofn))
		return filename;

	return L"";
}

#include <iostream>
#include <fstream>
STLGenerator::STLGenerator(HWND& hWnd, const V2D& lambda, const int width, const int height, const Space& space)
{
	const auto fname = OpenSaveDialog(hWnd, TEXT("STL file {*.stl}\0*.stl\0") TEXT("All files {*.*}\0*.*\0\0"));
	if (fname.size() <= 0) return;

	std::ofstream ofs(fname, std::ios::binary | std::ios::out);

	// ヘッダの書き込み
	char header[80] = "Lyapunov Fractal Viewer\nSoftware Auther: Eiichi Takebuchi (GRGSIBERIA)";
	ofs.write(reinterpret_cast<char*>(&header), 80);

	// 三角形の枚数の書き込み
	unsigned int num_tri = width * height * 12;
	ofs.write(reinterpret_cast<char*>(&num_tri), sizeof(unsigned int));

	/*
	*       P2-------------P3
	*      / |             /|
	*     /  |            / |
	*    P0--+-----------P1 |
	*    |   |           |  |
	*    |  P6-----------+-P7
	*    | /             | /
	*    |/              |/
	*    P4--------------P5
	* 
	* TOP, BOTTOM, FRONT, BACK, LEFT, RIGHT
	* 
	* TOP1 = 0,1,2
	* TOP2 = 1,2,3
	* BOTTOM1 = 4,5,6
	* BOTTOM2 = 5,6,7 ....
	*/

	const int faces[12][3] = {
		{ 0, 1, 2 },	// T1
		{ 1, 2, 3 },	// T2
		{ 4, 5, 6 },	// B1
		{ 5, 6, 7 },	// B2
		{ 0, 1, 4 },	// F1
		{ 4, 5, 1 },	// F2
		{ 2, 3, 6 },	// BK1
		{ 6, 7, 3 },	// BK2
		{ 0, 2, 4 },	// L1
		{ 4, 6, 2 },	// L2
		{ 1, 3, 5 },	// R1
		{ 5, 7, 3 }		// R2
	};

	float dirs[12][3] = {
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0,-1, 0 },
		{ 0,-1, 0 },
		{ 0, 0,-1 },
		{ 0, 0,-1 },
		{ 0, 0, 1 },
		{ 0, 0, 1 },
		{ -1,0, 0 },
		{ -1,0, 0 },
		{  1,0, 0 },
		{  1,0, 0 }
	};

	float diffW = space.width / width;
	float diffD = space.depth / height;
	float thick = space.thickness * 0.5f;

	V2D LAM = lambda;
	for (int D = 0; D < height; ++D)
		for (int W = 0; W < width; ++W)
			LAM[D][W] = std::isinf(lambda[D][W]) ? 0.f : lambda[D][W];

	float emax = 0.f;
	for (int i = 0; i < height; ++i)
		emax = max(emax, (float)*std::max_element(LAM[i].cbegin(), LAM[i].cend()));

	float emin = 0.f;
	for (int i = 0; i < height; ++i)
		emin = min(emin, (float)*std::min_element(LAM[i].cbegin(), LAM[i].cend()));

	float dist = space.height / (emax - emin);

	for (int D = 0; D < height; ++D) {
		for (int W = 0; W < width; ++W) {
			// 頂点の定義
			const float L = LAM[D][W];

			const float top = (L * dist) + thick;
			const float btm = (L * dist) - thick;

			const float pos[8][3] = {
				{ diffW * W, top, diffD * D},
				{ diffW * (W + 1), top, diffD * D},
				{ diffW * W, top, diffD * (D + 1)},
				{ diffW * (W + 1), top, diffD * (D + 1)},
				{ diffW * W, btm, diffD * D},
				{ diffW * (W + 1), btm, diffD * D},
				{ diffW * W, btm, diffD * (D + 1)},
				{ diffW * (W + 1), btm, diffD * (D + 1)},
			};

			// 三角形ごとに処理する
			for (int n = 0; n < 12; ++n) {
				// 法線を出力
				ofs.write((char*)(&dirs[n]), sizeof(float) * 3);
				
				// 頂点の座標を出力
				for (int i = 0; i < 3; ++i)
					ofs.write((char*)(&pos[faces[n][i]]), sizeof(float) * 3);

				// 未使用の領域
				const short zero = 0;
				ofs.write((char*)&zero, sizeof(short));
			}
		}
	}

	ofs.close();

	MessageBox(NULL, TEXT("Generated STL File"), TEXT("INFORMATION"), MB_OK | MB_ICONINFORMATION);
}
