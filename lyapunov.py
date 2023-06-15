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
    "-a", "--initial_a", type=float, required=True,
    help="float value [0,4]"
)
parser.add_argument(
    "-b", "--initial_b", type=float, required=True,
    help="float value [0,4]"
)
parser.add_argument(
    "-x", "--initial_x", type=float, default=0.5,
    help="Initial value for iterative calculation."
)
parser.add_argument(
    "-y", "--initial_y", type=float, default=0.5,
    help="Initial value for iterative calculation."
)
parser.add_argument(
    "-m", "--mode", type=str, choices=["3d", "plot"], default="plot",
    help="3D mode outputs files in STL format. In plot mode, draw figures with matplotlib."
)
parser.add_argument(
    "-w", "--width", type=int, default=512
)
parser.add_argument(
    "-h", "--height", type=int, default=512
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

def series2rvalue(series, width, height, a, b):
    rx = []
    ry = []
    ab = [b, a]
    for w in range(width):
        i = w % len(series)
        rx.append(ab[series[i]])
    for h in range(height):
        i = h % len(series)
        ry.append(ab[series[i]])
    return rx, ry

def r2x(r, initX):
    x = [initX]
    for i, _ in enumerate(r):
        x.append(x[i] * r[i] * (1. - x[i]))
    return x


if __name__ == "__main__":
    args = parser.parse_args()
    A = args.initial_a
    B = args.initial_b
    initX = args.initial_x
    initY = args.initial_y
    mode = args.mode
    seed = args.seed
    seq = args.sequencial
    width = args.width
    height = args.height

    # 系列の生成
    if seq == None:
        series = seed2series(seed)
    elif seed == -1:
        series = seq2series(seq)
    elif seq != None and seed != -1:
        raise ValueError(f"Both sequential and seed are specified. Please specify only one of them.")
    
    # 周期列の生成
    rx, ry = series2rvalue(series, width, height, A, B)

    # 漸化式を解く
    x = r2x(rx, initX)
    y = r2x(ry, initY)

    if mode == "3d":
        pass
    elif mode == "plot":
        pass
    else:
        raise ValueError(f"mode choices 3d or plot: {mode}")