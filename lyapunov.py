import numpy as np
import matplotlib.pyplot as plt
import argparse

parser = argparse.ArgumentParser(
    prog='Lyapunov Fractal',
    description='The Lyapunov fractal is a characteristic fractal figure that extends the logistic map. This program generates Lyapunov fractals.',
    epilog='Text at the bottom of help'
)
parser.add_argument(
    "-s", "--seed", type=int, default=None,
    help="seed generates a series based on the leading bit of the specified value. Higher numbers mean greater complexity."
)
parser.add_argument(
    "-seq", "--sequencial", type=str, default=None,
    help="sequential creates a cyclic sequence from a string consisting of arbitrary A and B."
)
parser.add_argument(
    "-n", "--num_iterations", type=int, default=10,
    help="Number of iterations."
)
parser.add_argument(
    "-x", "--initial_x", type=float, default=0.5,
    help="Initial value for iterative calculation."
)
parser.add_argument(
    "-m", "--mode", type=str, choices=["3d", "plot"], default="plot",
    help="3D mode outputs files in STL format. In plot mode, draw figures with matplotlib."
)
parser.add_argument(
    "-width", "--width", type=int, default=512
)
parser.add_argument(
    "-height", "--height", type=int, default=512
)
parser.add_argument(
    "-amax", "--amax", type=float, default=4
)
parser.add_argument(
    "-amin", "--amin", type=float, default=0
)
parser.add_argument(
    "-bmax", "--bmax", type=float, default=4
)
parser.add_argument(
    "-bmin", "--bmin", type=float, default=0
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

def r2x(r, initX):
    x = [initX]
    for i, _ in enumerate(r):
        x.append(x[i] * r[i] * (1. - x[i]))
    return x

def compute_lambda(r, x):
    import math
    total = 0.
    for n, _ in enumerate(r):
        absval = math.fabs(r[n] * (1. - (2. * x[n])))
        try:
            total += math.log(absval, math.e)
        except:
            continue
    return (1. / len(r)) * total



if __name__ == "__main__":
    args = parser.parse_args()
    initX = args.initial_x
    mode = args.mode
    seed = args.seed
    seq = args.sequencial
    width = args.width
    height = args.height
    N = args.num_iterations
    amin = args.amin
    amax = args.amax
    bmin = args.bmin
    bmax = args.bmax

    # 系列の生成
    series = None
    if seed != None:
        series = seed2series(seed)
    elif seq != None:
        series = seq2series(seq)
    elif seq == None and seed == -1:
        raise ValueError(f"Both sequential and seed are specified. Please specify only one of them.")
    
    a = np.linspace(amin, amax, width)
    b = np.linspace(bmin, bmax, height)
    dots = np.zeros(shape=(width, height))

    for i, bn in enumerate(b):
        for j, an in enumerate(a):
            r = series2rvalue(series, N, an, bn)
            x = r2x(r, initX)
            dots[i][j] = compute_lambda(r, x) * 8

    plt.figure()
    plt.imshow(dots)
    plt.colorbar()
    plt.tight_layout()
    plt.show()

    if mode == "3d":
        pass
    elif mode == "plot":
        pass
    else:
        raise ValueError(f"mode choices 3d or plot: {mode}")