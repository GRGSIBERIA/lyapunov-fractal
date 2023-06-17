import numpy as np
import matplotlib.pyplot as plt
import argparse
from numba import njit, f8
import toml

parser = argparse.ArgumentParser(
    prog='Lyapunov Fractal',
    description='The Lyapunov fractal is a characteristic fractal figure that extends the logistic map. This program generates Lyapunov fractals.',
    epilog='Text at the bottom of help'
)

parser.add_argument(
    "-i", "--input", type=str, required=True,
    help="toml settings, for example.toml"
)

def mode_plot():
    pass

def mode_3d():
    pass

def seed2series(seed: int):
    import sys
    import math
    
    bitmax = int(math.log2(sys.maxsize))
    bits = [1 << i for i in range(bitmax)]
    series = []
    for bit in reversed(bits):
        a = 1 if (seed & bit) > 0 else 0
        series.append(a)
    
    for i, bit in enumerate(series):
        if bit == 1:
            return series[i:]
    return series

def seq2series(seq):
    series = []
    for s in seq:
        if s == "A" or s == "1":
            series.append(1)
        elif s == "B" or s == "0":
            series.append(0)
    return series

def series2rvalue(series, N, a, b):
    r = []
    ab = [b, a]
    for i in range(N):
        idx = i % len(series)
        r.append(ab[series[idx]])
    return r


@njit(f8(f8, f8, f8))
def func_simple(x, r, _):
    return x * r * (1. - x)

@njit(f8(f8, f8, f8))
def func_sin2(x, r, const):
    return const * np.sin(x + r)**2.


def r2x(r, initX, const, func_mode):
    x = [initX]

    func = {
        "simple": func_simple,
        "sin2": func_sin2
    }
    func = func[func_mode]

    for i, _ in enumerate(r):
        x.append(func(*[x[i], r[i], const]))

    return x


@njit(f8(f8, f8, f8))
def grad_simple(x, r, _):
    return np.fabs(r * (1. - (2. * x)))

@njit(f8(f8, f8, f8))
def grad_sin2(x, r, const):
    return np.fabs(2. * const * np.sin(x + r) * np.cos(x + r))


def compute_lambda(r, x, const, func_mode):
    import math
    total = 0.

    func = {
        "simple": grad_simple,
        "sin2": grad_sin2
    }
    func = func[func_mode]
    
    for n, _ in enumerate(r):
        absval = func(*[x[n], r[n], const])
        try:
            total += math.log(absval, math.e)
        except:
            continue
    return (1. / len(r)) * total


if __name__ == "__main__":
    args = parser.parse_args()
    
    with open(args.input, "rt", encoding="utf-8") as f:
        tom = toml.load(f)
    
    pt = tom["plottings"]
    mode = pt["mode"]
    width = pt["width"]
    height = pt["height"]

    par = tom["parameters"]
    param = par["param"]
    initX = par["initX"]
    N = par["N"]
    amin = par["amin"]
    amax = par["amax"]
    bmin = par["bmin"]
    bmax = par["bmax"]
    func = par["func"]
    const = par["const"]

    # 系列の生成
    series = None
    if type(param) is int:
        series = seed2series(param)
    elif type(param) is str:
        series = seq2series(param)
    else:
        raise ValueError(f"Both sequential and seed are specified. Please specify only one of them.")
    
    a = np.linspace(amin, amax, width)
    b = np.linspace(bmin, bmax, height)
    dots = np.zeros(shape=(width, height))

    for i, bn in enumerate(b):
        for j, an in enumerate(a):
            r = series2rvalue(series, N, an, bn)
            x = r2x(r, initX, const, func)
            dots[i][j] = compute_lambda(r, x, const, func)

    

    if mode == "3d":
        pass
    elif mode == "plot":
        plt.figure()
        plt.imshow(dots)
        plt.colorbar()
        plt.tight_layout()
        plt.show()
    else:
        raise ValueError(f"mode choices 3d or plot: {mode}")