---
title: 区分線形凸関数
documentation_of: ../../src/Optimization/PiecewiseLinearConvexfunction.hpp
---
splay木でがんばった. 一般の min-plus 畳み込みは実装していない. \
メモリプールしている. static 関数 `clear` でノードリセット. 

## メンバ関数

| 関数名  | 内容|
| --- | ---|
| `PiecewiseLinearConvexfunction()`  | コンストラクタ $\displaystyle f(x) := 0$|
| `add_const(c)`      | $\displaystyle f(x)\leftarrow f(x)+c $ |
| `add_linear(a,c)` | $\displaystyle f(x)\leftarrow f(x) + ax+c$ |
| `shift(a)`   | $\displaystyle f(x)\leftarrow f(x-a)$|
| `add_relu(a,c)` |$\displaystyle f(x)\leftarrow f(x) + \begin{cases} 0 & x < c \newline a(x-c) & x \ge c \end{cases}$|
| `add_ax_bx_c(a,b,c)` | $\displaystyle f(x)\leftarrow f(x) + \begin{cases} a(x-c) & x < c \newline b(x-c) & x \ge c \end{cases}$ |
| `add_abs(a,c)` | $\displaystyle f(x)\leftarrow f(x) + a\lvert  x-c \rvert$|
| `chinfty_right(a)`  | $\displaystyle f(x)\leftarrow \begin{cases} f(x) & x \le a \newline \infty & x > a \end{cases} $ |
| `chinfty_left(a)`   | $\displaystyle f(x)\leftarrow \begin{cases} \infty & x < a \newline f(x) & x \ge a \end{cases} $ |
| `cumulative_chmin()`| $\displaystyle f(x)\leftarrow\min_{y\le x}f(y)$ |
| `cumulative_chmin_with_condition(a)`     | $\displaystyle f(x)\leftarrow\min_{y\le x \land y\le a}f(y)$  |
| `cumulative_chmin_rev()`   | $\displaystyle f(x)\leftarrow\min_{y\ge x}f(y)$ |
| `cumulative_chmin_rev_with_condition(a)` | $\displaystyle f(x)\leftarrow\min_{y\ge x \land y\ge a}f(y)$  |
| `chmin_sliding_window(a,b)`| $\displaystyle f(x)\leftarrow\min_{x-b\le y\le x-a}f(y)$      |
| `convex_convolution_with_ax_bx_c(a,b,c)` | $\displaystyle f(x)\leftarrow \min_y \left\lbrace f(x-y)+ \left(\begin{cases}  a(y-c) & y < c \newline b(y-c) & y \ge c \end{cases}\right)\right\rbrace$ |
| `convex_convolution_with_abs(a,c)`| $\displaystyle f(x)\leftarrow \min_y\left\lbrace f(x-y)+a\lvert y-c\rvert\right\rbrace$   |
| `operator+(f,g)`    | $f(x)+g(x)$ を返す. 破壊的なので注意  |
| `eval(a)`, `operator()(a)` | $f(a)$ を返す.   |
| `argmin()`   | 閉区間 $[l, r] = \lbrace y:f(y) = \min_x f(x)\rbrace$ を返す.     |
| `min()`      | $\min_x f(x)$ を返す.   |

## 問題例
[Kyoto University Programming Contest 2016 H - 壁壁壁壁壁壁壁](https://atcoder.jp/contests/kupc2016/tasks/kupc2016_h) \
[東京大学プログラミングコンテスト2012 L - じょうしょうツリー](https://atcoder.jp/contests/utpc2012/tasks/utpc2012_12) (`operator+` のverify)\
[第2回 ドワンゴからの挑戦状 予選 E - 花火](https://atcoder.jp/contests/dwango2016-prelims/tasks/dwango2016qual_e) \
[AtCoder Regular Contest 070 E - NarrowRectangles](https://atcoder.jp/contests/arc070/tasks/arc070_c) \
[AtCoder Beginner Contest 275 Ex - Monster](https://atcoder.jp/contests/abc275/tasks/abc275_h) (`operator+` のverify)

## 参考
[https://tokoharuland.hateblo.jp/entry/2019/12/25/000000](https://tokoharuland.hateblo.jp/entry/2019/12/25/000000)\
[https://maspypy.com/slope-trick-1-解説編](https://maspypy.com/slope-trick-1-%e8%a7%a3%e8%aa%ac%e7%b7%a8)\
[https://maspypy.com/slope-trick-2-問題編](https://maspypy.com/slope-trick-2-%e5%95%8f%e9%a1%8c%e7%b7%a8)\
[https://maspypy.com/slope-trick-3-slope-trick-の凸共役](https://maspypy.com/slope-trick-3-slope-trick-%e3%81%ae%e5%87%b8%e5%85%b1%e5%bd%b9)
