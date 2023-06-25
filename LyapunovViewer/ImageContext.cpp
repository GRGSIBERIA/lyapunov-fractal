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

const __m256 func_simple(const __m256& mx, const __m256& mr, const float c1, const float c2) {
	const auto one = _mm256_set1_ps(1.f);

	const auto sub1 = _mm256_sub_ps(one, mx);
	const auto mulrx = _mm256_mul_ps(mr, mx);
	return _mm256_mul_ps(mulrx, sub1);
}
const __m256 func_cyclic(const __m256& mx, const __m256& mr, const float c1, const float c2) {
	const auto xr = _mm256_add_ps(mx, mr);
	const auto sinxr = _mm256_sin_ps(xr);
	const auto two = _mm256_set1_ps(2.f);
	const auto constance = _mm256_broadcast_ss(&c1);

	const auto powsin = _mm256_pow_ps(sinxr, two);
	return _mm256_mul_ps(constance, powsin);
}
const __m256 grad_simple(const __m256& mx, const __m256& mr, const float c1, const float c2) {
	const auto one = _mm256_set1_ps(1.f);
	const auto two = _mm256_set1_ps(2.f);

	const auto x2 = _mm256_mul_ps(two, mx);
	const auto sx2 = _mm256_sub_ps(one, x2);
	const auto rs = _mm256_mul_ps(mr, sx2);
	
	const auto mask = _mm256_set1_ps(-0.f);
	const auto rsabs = _mm256_andnot_ps(mask, rs);
	return _mm256_log_ps(rsabs);
}
const __m256 grad_cyclic(const __m256& mx, const __m256& mr, const float c1, const float c2) {
	const auto xr = _mm256_add_ps(mx, mr);
	const auto s = _mm256_sin_ps(xr);
	const auto c = _mm256_cos_ps(xr);
	
	const auto two = _mm256_set1_ps(2.f);
	const auto mc1 = _mm256_set1_ps(c1);
	const auto t = _mm256_mul_ps(two, _mm256_mul_ps(mc1, _mm256_mul_ps(s, c)));

	const auto mask = _mm256_set1_ps(-0.f);
	const auto rsabs = _mm256_andnot_ps(mask, t);
	return _mm256_log_ps(rsabs);
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
	
	typedef const __m256(*F)(const __m256& mx, const __m256& mr, const float c1, const float c2);
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

#define UNLOOP(FUNC_NAME, RETURN_NAME) \
	for (int n = 1; n < N; ++n) {\
		

#pragma omp parallel for
	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; w += 8) {

			for (int i = 0; i < 8; ++i)
				x[h][w + i][0] = edit.PInitX;
				
			for (int n = 1; n < N; ++n) {
				__m256 mr, mx;
				for (int i = 0; i < 8; ++i) {
						mr.m256_f32[i] = r[h][w + i][n];
						mx.m256_f32[i] = x[h][w + i][n - 1];
				}
				const auto retval = func(mx, mr, edit.PConst1, edit.PConst2);

				for (int i = 0; i < 8; ++i)
					x[h][w + i][n] = retval.m256_f32[i];
			}
		}
	}
	
#pragma omp parallel for
	for (int h = 0; h < bufH; ++h) {
		for (int w = 0; w < bufW; w += 8) {
			for (int n = 1; n < N; ++n) {
				// lam[h][w][n] = grad(x[h][w][n], r[h][w][n], edit.PConst1, edit.PConst2);

				__m256 mr, mx;
				for (int i = 0; i < 8; ++i) {
					mr.m256_f32[i] = r[h][w + i][n];
					mx.m256_f32[i] = x[h][w + i][n];
				}
				const auto retval = grad(mx, mr, edit.PConst1, edit.PConst2);

				for (int i = 0; i < 8; ++i)
					lam[h][w + i][n] = retval.m256_f32[i];
				
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
