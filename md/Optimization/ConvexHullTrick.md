---
title: Convex-Hull-Trick
documentation_of: ../../src/Optimization/ConvexHullTrick.hpp
---

`ConvexHullTrick`は、直線の集合に対して、あるx座標での最小値（または最大値）を高速に求めるデータ構造です。
傾きの単調性やクエリの単調性は不要です。内部で`std::multiset`を使用しているため、挿入とクエリの計算量は $O(\log n)$ です。

## `ConvexHullTrick`

`y = kx + m` の形の直線を管理します。

**テンプレートパラメータ**
- `typename T`: 座標の型。整数型、浮動小数点数型の両方に対応しています。
- `MinMaxEnum obj`: `MINIMIZE`（最小値）か`MAXIMIZE`（最大値）を指定します。デフォルトは`MINIMIZE`です。

|メンバ関数|概要|計算量|
|---|---|---|
|`empty()`|直線が1本も挿入されていない場合に`true`を返します。||
|`insert(k,m)`|直線 $y=kx+m$ を挿入します。| $O(\log n)$|
|`query_line(x)` | $x$ における最小値(または最大値)をとる直線 $y=kx+m$ の係数 $(k,m)$ を返します。|$O(\log n)$| 
|`query(x)` | $x$ における最小値(または最大値) を返します。|$O(\log n)$| 

## `ConvexHullTrick_XY`

点の集合 $\{(a_i, b_i)\}$ があるとき、クエリ `(x, y)` に対して $a_i x + b_i y$ を最大化・最小化する $(a_i, b_i)$ を求める問題を高速に処理します。
これは、直線 $ax+by=c$ を考えたとき、点 $(x, y)$ を通る直線のc切片を最大・最小にする問題の双対と見なせます。

内部で `ConvexHullTrick<long double>` を利用しています。

**テンプレートパラメータ**
- `typename T`: 座標の型。

|メンバ関数|概要|計算量|
|---|---|---|
|`empty()`|点が1つも挿入されていない場合に`true`を返します。||
|`insert(a,b)`|点 $(a,b)$ を挿入します。| $O(\log n)$|
|`get_min(x,y)` | クエリ $(x,y)$ に対する $ax+by$ の最小値を返します。|$O(\log n)$| 
|`get_max(x,y)` | クエリ $(x,y)$ に対する $ax+by$ の最大値を返します。|$O(\log n)$| 

## Verify

- [Library Checker - Line Add Get Min](https://judge.yosupo.jp/problem/line_add_get_min) (`ConvexHullTrick`)
- [AtCoder Regular Contest 051 D - 長方形](https://atcoder.jp/contests/arc051/tasks/arc051_d) (`ConvexHullTrick_XY`)
- [AtCoder Beginner Contest 244 Ex - Linear Maximization](https://atcoder.jp/contests/abc244/tasks/abc244_h) (`ConvexHullTrick_XY`)
- [Yandex Contest Алгоритм 2022 K. Stepwise subsequence ](https://contest.yandex.com/contest/42710/problems/K) (有理数クラスで使用)

## 参考

- [https://github.com/kth-competitive-programming/kactl/blob/main/content/data-structures/LineContainer.h](https://github.com/kth-competitive-programming/kactl/blob/main/content/data-structures/LineContainer.h)
- [https://maspypy.github.io/library/convex/cht.hpp](https://maspypy.github.io/library/convex/cht.hpp)
