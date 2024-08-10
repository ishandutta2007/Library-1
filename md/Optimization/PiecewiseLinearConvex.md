---
title: 区分線形凸関数
documentation_of: ../../src/Optimization/PiecewiseLinearConvex.hpp
---

## `PiecewiseLinearConvex<T>` クラス
weight balanced tree でがんばった．一般の min-plus 畳み込みとかはない．\
メモリプールしている．static 関数 `reset` でノードリセット．

内部では $y$ 座標の相当する値を `T` より大きい型 `D` で管理している．例えば `T=long long` なら `D=__int128_t` とか．

区分線形凸関数 $f$ の接点の数を $n$ とする. 

| メンバ関数  | 内容| 計算量 |
| --- | ---| --- |
| `PiecewiseLinearConvex()`  | コンストラクタ $\displaystyle f(x) := 0$| $O(1)$ |
| `add_const(c)`      | $\displaystyle f(x)\leftarrow f(x)+c $ |$O(1)$ |
| `add_linear(a)` | $\displaystyle f(x)\leftarrow f(x) + ax$ | $O(1)$ |
| `add_max(a,b,x0)` |$\displaystyle f(x)\leftarrow f(x) + \max(a(x-x_0), b(x-x_0)) $ <br> ただし $a\lt b$ を要求|$O(\log n)$ |
| `add_ramp(a,x0)` | $\displaystyle f(x)\leftarrow f(x) + \max(a(x-x_0,0))$ |$O(\log n)$|
| `add_abs(a,x0)` | $\displaystyle f(x)\leftarrow f(x) + a\lvert  x-x_0 \rvert$ <br> ただし $a\ge0$を要求| $O(\log n)$|
| `add_inf(right=false, x0)`  | `right=false` のとき，$\displaystyle f(x)\leftarrow \begin{cases}  \infty  & x \lt x_0 \newline f(x) & x_0 \le x \end{cases} $ <br> `right=true` のとき，$\displaystyle f(x)\leftarrow \begin{cases} f(x) & x \le x_0 \newline \infty & x_0 \lt x \end{cases} $ |$O(\log n)$|
| `shift(a)`   | $\displaystyle f(x)\leftarrow f(x-a)$|$O(1)$|
| `chmin_cum(rev=false)`| `rev=false` のとき， $\displaystyle f(x)\leftarrow\min_{y\le x}f(y)$ <br> `rev=true` のとき，$\displaystyle f(x)\leftarrow\min_{y\ge x}f(y)$| $O(\log n)$|
| `chmin_slide_win(lb,ub)`| $\displaystyle f(x)\leftarrow\min_{x-\mathrm{ub}\le y\le x-\mathrm{lb}}f(y)= \min_{\mathrm{lb}\le y\le \mathrm{ub}} f(x-y)$ <br> ただし $\mathrm{lb}\le\mathrm{ub}$ を要求．|$O(\log n)$|
| `operator()(a)` | $f(a)$ を返す．<br> 返り値は `T` より大きい型 `D` (`T=long long`なら `__int128_t`) | $O(\log n)$ |
| `argmin()`   | 閉区間 $[l, r] = \lbrace y:f(y) = \min_x f(x)\rbrace$ を返す．| $O(\log n)$|
| `min()`      | $\min_x f(x)$ を返す.　<br> 返り値は `T` より大きい型 `D` (`T=long long`なら `__int128_t`) |$O(\log n)$|
| `size()`      |$f$ の接点の数 $n$ を返す．| $O(1)$ |
| `operator+=(g)`| $f(x)\leftarrow f(x)+g(x)$| $O(m\log (n+m))$ <br> ただし $g$ の接点の数を $m$ とした |
| `info()`| デバッグ用出力．返り値は `string`．| $O(n)$ |
| `dump_xs()`| $f$ の接点の $x$ 座標を返す．返り値は `vector<T>`．| $O(n)$ |
| `dump_xys()`| $f$ の接点の $x,y$ 座標を返す．返り値は `vector<pair<T,D>>` | $O(n)$ |
| `dump_slopes()`| $f$ の区分線形の 傾きを返す．返り値は `vector<T>` | $O(n)$ |


## 問題例
[Kyoto University Programming Contest 2016 H - 壁壁壁壁壁壁壁](https://atcoder.jp/contests/kupc2016/tasks/kupc2016_h) \
[東京大学プログラミングコンテスト2012 L - じょうしょうツリー](https://atcoder.jp/contests/utpc2012/tasks/utpc2012_12) (`operator+=` のverify)\
[第2回 ドワンゴからの挑戦状 予選 E - 花火](https://atcoder.jp/contests/dwango2016-prelims/tasks/dwango2016qual_e) \
[AtCoder Regular Contest 070 E - NarrowRectangles](https://atcoder.jp/contests/arc070/tasks/arc070_c)

## 参考
[https://tokoharuland.hateblo.jp/entry/2019/12/25/000000](https://tokoharuland.hateblo.jp/entry/2019/12/25/000000)\
[https://maspypy.com/slope-trick-1-解説編](https://maspypy.com/slope-trick-1-%e8%a7%a3%e8%aa%ac%e7%b7%a8)\
[https://maspypy.com/slope-trick-2-問題編](https://maspypy.com/slope-trick-2-%e5%95%8f%e9%a1%8c%e7%b7%a8)\
[https://maspypy.com/slope-trick-3-slope-trick-の凸共役](https://maspypy.com/slope-trick-3-slope-trick-%e3%81%ae%e5%87%b8%e5%85%b1%e5%bd%b9)