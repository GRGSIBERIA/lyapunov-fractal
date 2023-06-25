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
	const float sf = sinf(x + r);
	const float pf = powf(sf, 2.f);
	return c1 * pf;
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

	auto r = V3D(bufH, V2D(bufW, V1D(N, 0.f)));
	auto x = V3D(bufH, V2D(bufW, V1D(N, 0.f)));
	auto lam = V3D(bufH, V2D(bufW, V1D(N, 0.f)));
	lambda = V2D(bufH, V1D(bufW, 0.f));

	pixels = C2D(bufH, C1D(bufW, 0.f));
	
	typedef float(*F)(const float, const float, const float, const float);
	typedef std::pair<std::string, F> P;

	std::map<std::string, F> func_dic {
		P("simple", &func_simple),
		P("cyclic", &func_cyclic)
	};
	std::map<std::string, F> grad_dic{
		P("simple", &grad_simple),
		P("cyclic", &grad_cyclic)
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
	// A4B2 = AAAABB
	// �e�X�g�P�[�X
	// A15 = AAAAAAAAAAAAAAA
	auto sequence = std::string("");
	int numcnt = 0;
	char prev;
	for (int i = 0; i < edit.PSequence.size(); ++i) {
		const auto ps = edit.PSequence[i];

		if (ps == 'A') {
			sequence.push_back(ps);
			prev = ps;
		}
		else if (ps == 'B') {
			sequence.push_back(ps);
			prev = ps;
		}
		else {
			// ���������Ԃ��r�߂Ă���
			while (true) {
				const auto pp = edit.PSequence[i];
				if ('0' <= pp && pp <= '9') {
					if ('0' <= edit.PSequence[i - 1] && edit.PSequence[i - 1] <= '9')
					{
						/*
						11�̏��ԂȂ�A1 + 1 = 2�ƂȂ邽�߁A���O��1�O�ɐ������邩�ǂ����m�F���āA���l�Ȃ�10�i��������10�{����΂���
						A123�ł����Ă��A1, 1*10+2, 1*10^2+2*10+3=123
						*/
						numcnt *= 10;
					}
					numcnt += pp - (int)'0';

					++i;
				}
				else {
					--i;	// ��������Ȃ��Ƃ���ɓ˂�������������Y������1�߂�
					break;
				}
			}
			// �Ō��A,B�������Ƃ��̋L�����Ăі߂���sequence�ɒǉ�����
			for (int j = 1; j < numcnt; ++j) {
				sequence.push_back(prev);
			}
			numcnt = 0;
		}
	}

	// �V�[�P���X
	auto s = std::vector<int>();
	for (int i = 0; i < sequence.size(); ++i) {
		const char ps = sequence[i];
		if (ps == 'A') {
			s.push_back(1);
		}
		else if (ps == 'B') {
			s.push_back(0);
		}
	}

	// ����������
	auto S = std::vector<int>(edit.PN);
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


	/*
	if (edit.PFunc == "simple") {

#pragma omp parallel for
		for (int h = 0; h < bufH; ++h) {
			for (int w = 0; w < bufW; w += 4) {
				x[h][w][0] = edit.PInitX;



				for (int n = 1; n < N; ++n) {
					// x[n-1]�����Ă���̂́A�����l���v�Z�����邽��
					// x[h][w][n] = func(x[h][w][n - 1], r[h][w][n], edit.PConst1, edit.PConst2);

					//return r * x * (1.f - x);

					// r[h][w][n] * x[h][w][n-1] * (1.f - x[h][w][n]
					// x[h][w][n] = emm2
				}
			}
		}
	}
	*/

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
					pixels[h][w] = mincolor;
				}
				else if (lambda[h][w] < 0.f) {
					pixels[h][w] = mincolor;
				}
				else {
					const auto percent = lambda[h][w] * diffLAMMAX;
					RGBRGB(R); RGBRGB(G); RGBRGB(B);
					pixels[h][w] = RGB(R, G, B);
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
					pixels[h][w] = mincolor;
				}
				else {
					const auto lam = lambda[h][w];
					const auto percent = (lam - lammin) * dif;

					RGBRGB(R); RGBRGB(G); RGBRGB(B);
					const auto rgb = RGB((int)R, (int)G, (int)B);
					pixels[h][w] = rgb;
				}
			}
		}
	}
	static int cnt = 0;
	// �o�b�t�@�ɐF��]������
	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; ++w) {
			const auto color = pixels[h][w];

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
