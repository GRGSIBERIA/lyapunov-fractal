#include "ImageContext.h"

void ImageContext::initialize(HWND& hWnd, const int width, const int height)
{
	bufW = width;
	bufH = height;
	destroy();

	HDC hdc = GetDC(hWnd);

	bitmap = CreateCompatibleBitmap(hdc, bufW, bufH);
	buffer = CreateCompatibleDC(hdc);

	SelectObject(buffer, bitmap);
	SelectObject(buffer, GetStockObject(NULL_PEN));

	PatBlt(buffer, 0, 0, bufW, bufH, WHITENESS);

	ReleaseDC(hWnd, hdc);
}

#include <vector>
void ImageContext::generate(const EditContext& edit)
{
	using namespace std;
	auto r = vector<vector<vector<float>>>(bufH, vector<vector<float>>(bufW, vector<float>(edit.PN + 1)));
	auto x = vector<vector<vector<float>>>(bufH, vector<vector<float>>(bufW, vector<float>(edit.PN + 1)));
	auto lam = vector<vector<vector<float>>>(bufH, vector<vector<float>>(bufW, vector<float>(edit.PN + 1)));
	lambda = vector<vector<float>>(bufH, vector<float>(bufW));
	pixcels = vector<vector<COLORREF>>(bufH, vector<COLORREF>(bufW));
	const auto N = edit.PN + 1;
	// N + 1にしているのは、rとxで系列長が合わないため

	auto func_simple = [](const float x, const float r, const float c1, const float c2) -> float {
		return r * x * (1.0f - x);
	};
	auto func_cyclic = [](const float x, const float r, const float c1, const float c2) -> float {
		return c1 * powf(sinf(x + r), 2.0f);
	};
	auto grad_simple = [](const float x, const float r, const float c1, const float c2) -> float {
		return logf(fabsf(r * (1.0f - 2.0f * x)));
	};
	auto grad_cyclic = [](const float x, const float r, const float c1, const float c2) -> float {
		return logf(fabsf(2.0f * c1 * sinf(x + r) * cosf(x + r)));
	};
	auto func = edit.PFunc.compare("simple") ? func_simple : (edit.PFunc.compare("sin2") ? func_cyclic : nullptr);
	auto grad = edit.PFunc.compare("simple") ? grad_simple : (edit.PFunc.compare("sin2") ? grad_cyclic : nullptr);

	// nullptrの可能性のある関数があるため、この分岐なしでは警告がでる
	// or で繋げたいが、繋げてしまうとgradで警告が出たままになるため、わざわざ分けて書いている
	if (func == nullptr) {
		MessageBox(NULL, TEXT("INVALID FUNCTION NAME"), TEXT("CAUGHT ERROR"), MB_OK | MB_ICONERROR);
		return;
	}
	if (grad == nullptr) {
		MessageBox(NULL, TEXT("INVALID FUNCTION NAME"), TEXT("CAUGHT ERROR"), MB_OK | MB_ICONERROR);
		return;
	}

	// シーケンスを作る
	auto s = vector<int>();
	for (int i = 0; i < edit.PSequence.size(); ++i) {
		if (edit.PSequence[i] == 'A') {
			s.push_back(1);
		}
		else {
			s.push_back(0);
		}
	}

	// 周期列を作る
	auto S = vector<int>(edit.PN);
	for (int i = 0; i < edit.PN; ++i) {
		S[i] = s[(size_t)i % s.size()];
	}

	// 縦横の差分
	const float da = (edit.PAmax - edit.PAmin) / (float)edit.PWidth;
	const float db = (edit.PBmax - edit.PBmin) / (float)edit.PHeight;

	// ここから先、キャッシュヒットしやすくするため、処理ごとにループを実行する
#pragma omp parallel for
	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; ++w) {
			for (int n = 1; n < N; ++n) {
				r[h][w][n] = S[n] ? edit.PAmin + da * w : edit.PBmin + db * h;
			}
		}
	}

#pragma omp parallel for
	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; ++w) {
			x[h][w][0] = edit.PInitX;

			for (int n = 1; n < N; ++n) {
				x[h][w][n] = func(x[h][w][n], r[h][w][n], edit.PConst1, edit.PConst2);
			}
		}
	}

	const float dN = 1.0f / edit.PN;
#pragma omp parallel for
	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; ++w) {
			for (int n = 1; n < N; ++n) {
				lam[h][w][n] = grad(x[h][w][n], r[h][w][n], edit.PConst1, edit.PConst2);
			}
		}
	}

#pragma omp parallel for
	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; ++w) {
			// 添え字の0番目で集計する
			lam[h][w][0] = 0.0;
			for (int n = 1; n < N; ++n) {
				lam[h][w][0] += lam[h][w][n];
			}
			lam[h][w][0] *= dN;
		}
	}
	
	// 最小値と最大値を求める
	float lammax = 0.0f;
	float lammin = 0.0f;

#pragma omp parallel for
	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; ++w) {
			if (lammax < lam[h][w][0]) {
				lammax = lam[h][w][0];
			}
			if (lammin > lam[h][w][0]) {
				lammin = lam[h][w][0];
			}
			// lam[h][w][0]の結果を画像に書き込む
			lambda[h][w] = lam[h][w][0];
		}
	}
}

void ImageContext::draw(HDC& hdc)
{
	SelectObject(buffer, bitmap);

#define RGBRGB(X) const auto X = (Get##X##Value(maxcolor) - Get##X##Value(mincolor)) * percent + Get##X##Value(mincolor)
	
	// カオスの場合は最小色で塗りつぶす
	if (isChaos) {
#pragma omp parallel for
		for (int h = 0; h < bufH; ++h) {
			for (int w = 0; w < bufW; ++w) {
				if (lambda[h][w] < 0.f) pixcels[h][w] = mincolor;
				else {
					const float diffLAMMAX = 1.f / lammax;
					const auto percent = lambda[h][w] * diffLAMMAX;
					RGBRGB(R); RGBRGB(G); RGBRGB(B);
					pixcels[h][w] = RGB(R, G, B);
				}
			}
		}
	}
	else {
		// 色を求める
#pragma omp parallel for
		for (int h = 0; h < bufH; ++h) {
			for (int w = 0; w < bufW; ++w) {
				const float sub = lammax - lammin;
				const float dif = 1.f / sub;
				const auto percent = (lambda[h][w] + lammin) * dif;

				RGBRGB(R); RGBRGB(G); RGBRGB(B);
				pixcels[h][w] = RGB(R, G, B);

				//SetPixel(buffer, w, h, color);
			}
		}
	}

	

	// バッファに色を転送する
	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; ++w) {
			SetPixel(buffer, w, h, pixcels[h][w]);
		}
	}

	//BitBlt(hdc, 320 + 24, 42, width, height, buffer, 0, 0, SRCCOPY);
	SetStretchBltMode(hdc, COLORONCOLOR);
	StretchBlt(hdc, 320 + 24, 42, curW, curH, buffer, 0, 0, bufW, bufH, SRCCOPY);

	TextOut(hdc, 324, 300, L"B", lstrlen(L"B"));
	TextOut(hdc, 320 + 256, 560, L"A", lstrlen(L"A"));
	TextOut(hdc, 324 - 8, 32 + 16, L"min", lstrlen(L"min"));
	TextOut(hdc, 324 - 12, 512 + 16, L"max", lstrlen(L"max"));
	TextOut(hdc, 324 + 24, 560, L"min", lstrlen(L"min"));
	TextOut(hdc, 324 + 512 - 12, 560, L"max", lstrlen(L"max"));
}

void ImageContext::destroy()
{
	DeleteObject(bitmap);
}
