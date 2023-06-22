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

float func_simple(const float x, const float r, const float c1, const float c2) {
	return r * x * (1.f - x);
}
float func_cyclic(const float x, const float r, const float c1, const float c2){
	return c1 * powf(sinf(x + r), 2.f);
}
float grad_simple(const float x, const float r, const float c1, const float c2){
	const float x2 = 2.f * x;
	const float sx2 = 1.f - x2;
	const float rs = r * sx2;
	const float rsabs = fabsf(rs);
	const float logabs = logf(rsabs);
	return logabs;
}
float grad_cyclic(const float x, const float r, const float c1, const float c2){
	const float s = sinf(x + r);
	const float c = cosf(x + r);
	const float t = 2.f * c1 * s * c;
	const float abst = fabs(t);
	const float logabs = logf(abst);
	return logabs;
}

#include <vector>
#include <map>
void ImageContext::generate(HWND& hWnd, const EditContext& edit)
{
	const auto N = edit.PN + 1;	// N + 1にしているのは、rとxで系列長が合わないため

	using namespace std;
	typedef vector<vector<vector<float>>> D3;
	typedef vector<vector<float>> D2;
	typedef vector<float> D1;

	auto r = D3(bufH, D2(bufW, D1(N, 0.f)));
	auto x = D3(bufH, D2(bufW, D1(N, 0.f)));
	auto lam = D3(bufH, D2(bufW, D1(N, 0.f)));
	lambda = D2(bufH, D1(bufW, 0.f));

	pixcels = vector<vector<COLORREF>>(bufH, vector<COLORREF>(bufW, 0.f));
	
	
	typedef float(*F)(const float, const float, const float, const float);
	typedef std::pair<std::string, F> P;

	std::map<std::string, F> func_dic {
		P("simple", &func_simple),
		P("sin2", &func_cyclic)
	};
	std::map<std::string, F> grad_dic{
		P("simple", &grad_simple),
		P("sin2", &grad_cyclic)
	};
	auto func = func_dic[edit.PFunc];
	auto grad = grad_dic[edit.PFunc];

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
		const char ps = edit.PSequence[i];
		if (ps == 'A') {
			s.push_back(1);
		}
		else if (ps == 'B') {
			s.push_back(0);
		}
		else {
			// A,Bのあとに数字が来たら、その数字の回数だけ繰り返す
			int n = 0;
			int j = 0;
			while (true) {
				const char pp = edit.PSequence[i+j];
				if ('0' <= pp && pp <= '9') {
					n *= 10;
					n += pp - (int)'0';
				}
				else {
					break;
				}
				++j;
			}
			for (int k = 0; k < n; ++k) {
				s.push_back(edit.PSequence[i - 1]);
			}
			/*
			* s A62B
			* i  1
			* j  01
			* i   23
			*/
			i += j;
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
				r[h][w][n] = S[n - 1] ? edit.PAmin + da * (float)w : edit.PBmin + db * (float)h;
			}
		}
	}

#pragma omp parallel for
	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; ++w) {
			x[h][w][0] = edit.PInitX;

			for (int n = 1; n < N; ++n) {
				// x[n-1]をしているのは、初期値を計算させるため
				x[h][w][n] = func(x[h][w][n-1], r[h][w][n], edit.PConst1, edit.PConst2);
			}
		}
	}

#pragma omp parallel for
	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; ++w) {
			for (int n = 1; n < N; ++n) {
				lam[h][w][n] = grad(x[h][w][n], r[h][w][n], edit.PConst1, edit.PConst2);
			}
		}
	}

	const float dN = 1.0f / edit.PN;
#pragma omp parallel for	// ここはparallel for があるとまずい
	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; ++w) {
			// 添え字の0番目で集計する
			lam[h][w][0] = 0.0;
			for (int n = 1; n < N; ++n) {
				lam[h][w][0] += lam[h][w][n];
			}
			lambda[h][w] = lam[h][w][0] * dN;
		}
	}
	
	// 最小値と最大値を求める
	lammax = lambda[0][0];
	lammin = 0.f;

	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; ++w) {
			if (lammax < lambda[h][w]) {
				lammax = lambda[h][w];
			}
			// 負の無限大が入ることがあるので、負の無限大は別の処理に
			if (lammin > lambda[h][w] && lambda[h][w] != -INFINITY) {
				lammin = lambda[h][w];
			}
		}
	}

	SelectObject(buffer, bitmap);

	// byteに直す必要があった
#define RGBRGB(X) const std::byte X = (std::byte)roundf(((float)Get##X##Value(maxcolor) - (float)Get##X##Value(mincolor)) * percent + (float)Get##X##Value(mincolor))

	const float sub = lammax - lammin;
	const float dif = 1.f / sub;
	const float diffLAMMAX = 1.f / lammax;

	// カオスの場合は最小色で塗りつぶす
	if (isChaos) {
#pragma omp parallel for
		for (int h = 0; h < bufH; ++h) {
			for (int w = 0; w < bufW; ++w) {
				if (lambda[h][w] == -INFINITY) {
					pixcels[h][w] = mincolor;
				}
				else if (lambda[h][w] < 0.f) {
					pixcels[h][w] = mincolor;
				}
				else {
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
				if (lambda[h][w] == -INFINITY) {
					pixcels[h][w] = mincolor;
				}
				else {
					const auto lam = lambda[h][w];
					const auto percent = (lam - lammin) * dif;

					RGBRGB(R); RGBRGB(G); RGBRGB(B);
					const auto rgb = RGB((int)R, (int)G, (int)B);
					pixcels[h][w] = rgb;
				}
			}
		}
	}
	static int cnt = 0;
	// バッファに色を転送する
	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; ++w) {
			const auto color = pixcels[h][w];

			SetPixel(buffer, w, h, color);
		}
	}

	// InvalidateRectをかけないとbufferに色が転送されない
	InvalidateRect(hWnd, NULL, FALSE);
}

#include <format>
void ImageContext::draw(HDC& hdc, HWND& hWnd)
{
	const int cx = 344;
	const int cy = 42;

	SetStretchBltMode(buffer, COLORONCOLOR);
	StretchBlt(hdc, cx, cy, curW, curH, buffer, 0, 0, bufW, bufH, SRCCOPY);
	//BitBlt(hdc, 320 + 24, 42, curW, curH, buffer, 0, 0, SRCCOPY);

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
	DeleteDC(buffer);
}
