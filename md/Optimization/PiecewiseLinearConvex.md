---
title: 区分線形凸関数
documentation_of: ../../src/Optimization/PiecewiseLinearConvex.hpp
---

## `PiecewiseLinearConvex<T, persistent, NODE_SIZE>` クラス

区分線形凸関数 $f(x)$ ．

slope trick を weight balanced tree で実装したもの．一般の min-plus 畳み込みとかはない．\
メモリプールしている．static 関数 `reset` でノードリセット．

`T` は $x$ 座標および傾きに相当する値の型．
内部では $y$ 座標に相当する値を `T` より大きい型 `D` で管理している．例えば `T=long long` なら `D=__int128_t` とか．浮動小数点数もたぶん使える．

テンプレートの第二引数を `true` にすることで永続化する．

区分線形凸関数 $f$ の接点の数を $n$ とする. 

### コンストラクタ

||内容|計算量|
|---|---|---|
| `PiecewiseLinearConvex()`  | コンストラクタ $\displaystyle f(x) := 0$.| $O(1)$ |
|`PiecewiseLinearConvex(v)`|コンストラクタ $\displaystyle f(x) := \sum_{i=1}^n \max(0, a_i(x-x_i))$. <br> 引数は `std::vector<std::pair<T,T>>` で，$\lbrace(a_1,x_1),\dots,(a_n,x_n)\rbrace$ を渡す．|$O(n)$|

### static メンバ関数

|名前|内容|
|---|---|
|`pool_empty()`|ノードプールがいっぱいになりそうだったら `true` を返す．|
|`reset()`|ノードプールをリセット．|
| 1. `rebuild(plc...)` <br>2. `rebuild(plcs)`|ノードプールをリセットして，参照渡しで引数に与えた `PiecewiseLinearConvex` インスタンスは再構築復元する．<br> 1. 可変引数 `PiecewiseLinearConvex&...` <br> 2. `std::vector<PiecewiseLinearConvex>&`|

### メンバ関数

| 名前  | 内容| 計算量 |
| --- | ---| --- |
| `add_const(c)`      | $\displaystyle f(x)\leftarrow f(x)+c $ |$O(1)$ |
| `add_linear(a)` | $\displaystyle f(x)\leftarrow f(x) + ax$ | $O(1)$ |
| `add_max(a,b,x0)` |$\displaystyle f(x)\leftarrow f(x) + \max(a(x-x_0), b(x-x_0)) $ <br> ただし $a\lt b$ を要求|$O(\log n)$ |
| `add_ramp(a,x0)` | $\displaystyle f(x)\leftarrow f(x) + \max(a(x-x_0),0)$ |$O(\log n)$|
| `add_abs(a,x0)` | $\displaystyle f(x)\leftarrow f(x) + a\lvert  x-x_0 \rvert$ <br> ただし $a\ge0$を要求| $O(\log n)$|
| `subtract_max(a,b,x0)` |$\displaystyle f(x)\leftarrow f(x) - \max(a(x-x_0), b(x-x_0)) $ <br> ただし $a\lt b$ を要求<br> $f$ が凸性を保てないとassertで落ちる．<br>$\sum_{i} \lvert x-x_i\rvert$ から $\sum_{i\ne j} \lvert x-x_i\rvert$<br>を求めるときなどに使う想定．|$O(\log n)$ |
| `subtract_ramp(a,x0)` | $\displaystyle f(x)\leftarrow f(x) - \max(a(x-x_0),0)$ <br> $f$ が凸性を保てないとassertで落ちる．|$O(\log n)$|
| `subtract_abs(a,x0)` | $\displaystyle f(x)\leftarrow f(x) - a\lvert  x-x_0 \rvert$ <br> ただし $a\ge0$を要求<br> $f$ が凸性を保てないとassertで落ちる．| $O(\log n)$|
| `add_inf(right=false, x0)`  | `right=false` のとき，<br> $\displaystyle f(x)\leftarrow \begin{cases}  \infty  & x \lt x_0 \newline f(x) & x_0 \le x \end{cases} $ <br> `right=true` のとき，<br> $\displaystyle f(x)\leftarrow \begin{cases} f(x) & x \le x_0 \newline \infty & x_0 \lt x \end{cases} $ |$O(\log n)$|
| `shift(a)`   | $\displaystyle f(x)\leftarrow f(x-a)$|$O(1)$|
| `chmin_cum(rev=false)`| `rev=false` のとき， $\displaystyle f(x)\leftarrow\min_{y\le x}f(y)$ <br> `rev=true` のとき，$\displaystyle f(x)\leftarrow\min_{y\ge x}f(y)$| $O(\log n)$|
| `chmin_slide_win(lb,ub)`| $\displaystyle f(x)\leftarrow\min_{x-\mathrm{ub}\le y\le x-\mathrm{lb}}f(y)= \min_{\mathrm{lb}\le y\le \mathrm{ub}} f(x-y)$ <br> ただし $\mathrm{lb}\le\mathrm{ub}$ を要求．|$O(\log n)$|
| `operator()(a)` | $f(a)$ を返す．<br> 返り値は `optional<D>`．存在しない場合 ($\infty$の場合) 無効値 (`nullopt`) を返す．<br> `D` は `T` より大きい型 (`T=long long`なら `D=__int128_t`) | $O(\log n)$ |
| `argmin()`   | 閉区間 $[l, r] = \lbrace y:f(y) = \min_x f(x)\rbrace$ を返す．<br> ただし存在しない場合，invalidな値($\lbrack 1,0\rbrack$)を返す．| $O(\log n)$|
| `min()`      | $\min_x f(x)$ を返す.　<br> 返り値は `optional<D>`．存在しない場合無効値 (`nullopt`) を返す．<br> `D` は `T` より大きい型 (`T=long long`なら `D=__int128_t`) |$O(\log n)$|
| `size()`      |$f$ の接点の数 $n$ を返す．| $O(1)$ |
| `operator+(g)`| $f(x)+g(x)$ を返す．<br> 永続化してないと破壊的．| $\displaystyle O\left(m\log \left(\frac{n}{m}+1\right)\right)$ <br> ただし $f,g$ の接点の数の小さい方を $m$, 大きい方を $n$ とした． |
| `operator+=(g)`| $f(x)\leftarrow f(x)+g(x)$　<br> 永続化してないと $g$ は破壊される．| $\displaystyle O\left(m\log \left(\frac{n}{m}+1\right)\right)$ <br> ただし $f,g$ の接点の数の小さい方を $m$, 大きい方を $n$ とした． |
| `info()`| デバッグ用出力．返り値は `string`．| $O(n)$ |
| `dump_xs()`| $f$ の接点の $x$ 座標を返す．返り値は `vector<T>`．| $O(n)$ |
| `dump_xys()`| $f$ の接点の $x,y$ 座標を返す．返り値は `vector<pair<T,D>>` | $O(n)$ |
| `dump_slopes()`| $f$ の区分線形の 傾きを返す．返り値は `vector<T>` | $O(n)$ |

## Verify
- [Kyoto University Programming Contest 2016 H - 壁壁壁壁壁壁壁](https://atcoder.jp/contests/kupc2016/tasks/kupc2016_h) 
- [東京大学プログラミングコンテスト2012 L - じょうしょうツリー](https://atcoder.jp/contests/utpc2012/tasks/utpc2012_12) (`operator+=`)
- [第2回 ドワンゴからの挑戦状 予選 E - 花火](https://atcoder.jp/contests/dwango2016-prelims/tasks/dwango2016qual_e) 
- [CSES Increasing Array II](https://cses.fi/problemset/task/2132/) 
- [Codeforces Round 371 (Div. 1) C. Sonya and Problem Wihtout a Legend](https://codeforces.com/contest/713/problem/C)
- [2020 Petrozavodsk Winter Camp, Jagiellonian U Contest C. Bookface](https://codeforces.com/gym/102576/problem/C)
- [MemSQL Start[c]UP 3.0 - Round 2 and Codeforces Round 437 (Div. 1) D. Buy Low Sell High](https://codeforces.com/contest/866/problem/D)
- [Helvetic Coding Contest 2017 N. April Fools' Problem (medium)](https://codeforces.com/contest/802/problem/N) (hard は TLE) 
- [Codeforces Round 584 - Dasha Code Championship - Elimination Round (rated, open for everyone, Div. 1 + Div. 2)](https://codeforces.com/contest/1209/problem/H) (`long double`)
- [LibreOJ #2419. 「USACO 2016 US Open, Platinum」Landscaping](https://loj.ac/p/2419)
- [Singapore NOI 2018 safety](https://oj.uz/problem/view/NOI18_safety) 
- [LMIO 2019 Potatoes and fertilizers](https://oj.uz/problem/view/LMIO19_bulves) 
- [APIO 2016 P2 — Fireworks](https://oj.uz/problem/view/APIO16_fireworks) (`operator+=`)
- [CodeChef CCDSAP Exam](https://www.codechef.com/problems/CCDSAP)
- [CodeChef Manufacturing Goods](https://www.codechef.com/problems/CZMG17) (解なし判定あり)
- [CodeChef Tangling Tree](https://www.codechef.com/problems/TANGLINGTREE) (`operator+=`)
- [CodeChef Tree Balancing](https://www.codechef.com/problems/TREEBAL) (永続，`operator+=`)
- [ICPC WF Conquer The World](https://icpc.kattis.com/problems/conquertheworld) (`operator+=`)

## 参考
- [https://tokoharuland.hateblo.jp/entry/2019/12/25/000000](https://tokoharuland.hateblo.jp/entry/2019/12/25/000000)
- [https://maspypy.com/slope-trick-1-解説編](https://maspypy.com/slope-trick-1-%e8%a7%a3%e8%aa%ac%e7%b7%a8)
- [https://maspypy.com/slope-trick-2-問題編](https://maspypy.com/slope-trick-2-%e5%95%8f%e9%a1%8c%e7%b7%a8)
- [https://maspypy.com/slope-trick-3-slope-trick-の凸共役](https://maspypy.com/slope-trick-3-slope-trick-%e3%81%ae%e5%87%b8%e5%85%b1%e5%bd%b9)