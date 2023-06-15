import numpy as np
import matplotlib.pyplot as plt
import argparse

parser = argparse.ArgumentParser(
    prog='Lyapunov Fractal',
    description='The Lyapunov fractal is a characteristic fractal figure that extends the logistic map. This program generates Lyapunov fractals.',
    epilog='Text at the bottom of help')
parser.add_argument(
    "-s", "--seed", type=int, default=-1,
    help="seed generates a series based on the leading bit of the specified value. Higher numbers mean greater complexity.")
parser.add_argument(
    "-a", "--initial_a", type=float, required=True,
    help="[0,4]")
parser.add_argument(
    "-b", "--initial_b", type=float, required=True,
    help="[0,4]")
parser.add_argument(
    "-x", "--initial_x", type=float, default=0.5,
    help="Initial value for iterative calculation.")
parser.add_argument(
    "-m", "--mode", type=str, choices=["3d", "plot"], default="plot",
    help="3D mode outputs files in STL format. In plot mode, draw figures with matplotlib.")


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

if __name__ == "__main__":
    args = parser.parse_args()
    initA = args.initial_a
    initB = args.initial_b
    initX = args.initial_x
    mode = args.mode
    seed = args.seed

    series = seed2series(seed)
    print(series)

    if mode == "3d":
        pass
    elif mode == "plot":
        pass
    else:
        raise ValueError(f"mode choices 3d or plot: {mode}")