## 計算方法

1. 任意の長さのAとBからなる系列 $L$ を作る。
1. 系列 $L$ より、任意の長さの周期列 $S$ を作る。$S$ は$L$を繰り返して作る。$L = AABB$ で長さ $6$ の周期列を作る場合、$S = AABBAA$。
1. $(a,b)$の組を $a \in [0,4], b \in [0,4]$ から選ぶ。
1. $r_n$ は、$a, b$からなる系列であり、長さは周期列 $S$ と一致する。$S_n = A \rightarrow r_n = a$ もしくは $S_n = B \rightarrow r_n = b$
1. $r_n$ から漸化式 $x_n$ を解く。漸化式の初期値が $0.5$ であるならば $x_0 = 0.5$ となり、$x_{n+1} = r_n x_n (1 - x_n)$
1. リアプノフ指数 $\lambda$ を計算する。リアプノフ指数は下式で表す。
$$
\lambda = \lim_{N \rightarrow \infty} \frac{1}{N} \sum^N_{n=1} \log \left|\frac{dx_{n+1}}{dx_n}\right| = \lim_{N \rightarrow \infty} \frac{1}{N} \sum^N_{n=1} \log \left|r_n (1 - 2x_n)\right|
$$
7. リアプノフ指数をもとに