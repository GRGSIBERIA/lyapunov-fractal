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
	const auto N = edit.PN + 1;	// N + 1�ɂ��Ă���̂́Ar��x�Ōn�񒷂�����Ȃ�����

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

	// nullptr�̉\���̂���֐������邽�߁A���̕���Ȃ��ł͌x�����ł�
	// or �Ōq���������A�q���Ă��܂���grad�Ōx�����o���܂܂ɂȂ邽�߁A�킴�킴�����ď����Ă���
	if (func == nullptr) {
		MessageBox(NULL, TEXT("INVALID FUNCTION NAME"), TEXT("CAUGHT ERROR"), MB_OK | MB_ICONERROR);
		return;
	}
	if (grad == nullptr) {
		MessageBox(NULL, TEXT("INVALID FUNCTION NAME"), TEXT("CAUGHT ERROR"), MB_OK | MB_ICONERROR);
		return;
	}

	// �V�[�P���X�����
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
			// A,B�̂��Ƃɐ�����������A���̐����̉񐔂����J��Ԃ�
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

	// ����������
	auto S = vector<int>(edit.PN);
	for (int i = 0; i < edit.PN; ++i) {
		S[i] = s[(size_t)i % s.size()];
	}

	// �c���̍���
	const float da = (edit.PAmax - edit.PAmin) / (float)edit.PWidth;
	const float db = (edit.PBmax - edit.PBmin) / (float)edit.PHeight;

	// ���������A�L���b�V���q�b�g���₷�����邽�߁A�������ƂɃ��[�v�����s����
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
				// x[n-1]�����Ă���̂́A�����l���v�Z�����邽��
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
#pragma omp parallel for	// ������parallel for ������Ƃ܂���
	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; ++w) {
			// �Y������0�ԖڂŏW�v����
			lam[h][w][0] = 0.0;
			for (int n = 1; n < N; ++n) {
				lam[h][w][0] += lam[h][w][n];
			}
			lambda[h][w] = lam[h][w][0] * dN;
		}
	}
	
	// �ŏ��l�ƍő�l�����߂�
	lammax = lambda[0][0];
	lammin = 0.f;

	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; ++w) {
			if (lammax < lambda[h][w]) {
				lammax = lambda[h][w];
			}
			// ���̖����傪���邱�Ƃ�����̂ŁA���̖�����͕ʂ̏�����
			if (lammin > lambda[h][w] && lambda[h][w] != -INFINITY) {
				lammin = lambda[h][w];
			}
		}
	}

	SelectObject(buffer, bitmap);

	// byte�ɒ����K�v��������
#define RGBRGB(X) const std::byte X = (std::byte)roundf(((float)Get##X##Value(maxcolor) - (float)Get##X##Value(mincolor)) * percent + (float)Get##X##Value(mincolor))

	const float sub = lammax - lammin;
	const float dif = 1.f / sub;
	const float diffLAMMAX = 1.f / lammax;

	// �J�I�X�̏ꍇ�͍ŏ��F�œh��Ԃ�
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
		// �F�����߂�
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
	// �o�b�t�@�ɐF��]������
	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; ++w) {
			const auto color = pixcels[h][w];

			SetPixel(buffer, w, h, color);
		}
	}

	// InvalidateRect�������Ȃ���buffer�ɐF���]������Ȃ�
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
