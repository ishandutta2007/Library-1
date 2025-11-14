---
title: Convex-Hull-Trick
documentation_of: ../../src/Optimization/ConvexHullTrick.hpp
---
浮動小数点数も行けるはず

## `ConvexHullTrick`
templateの第二引数で最大最小を指定. デフォルトは最小値取得.

|メンバ関数|概要|計算量|
|---|---|---|
|`empty()`|何も直線を挿入していないなら true||
|`insert(k,m)`|直線 $y=kx+m$ を挿入 | $O(\log n)$　|
|`query_line(x)` | $x$ における最小値(最大値)をとる直線 $y=kx+m$ の係数 $(k,m)$ を返す. |$O(\log n)$ | 
|`query(x)` | $x$ における最小値(最大値) を返す |$O(\log n)$ | 

## `ConvexHullTrick_XY`

|メンバ関数|概要|計算量|
|---|---|---|
|`empty()`|何も直線を挿入していないなら true||
|`insert(a,b)`|直線 $ax+by$ を挿入 | $O(\log n)$　|
|`get_min(x,y)` | $(x,y)$ における最小値を返す.  |$O(\log n)$ | 
|`get_max(x,y)` | $(x,y)$ における最大値を返す.  |$O(\log n)$ | 

## Verify

- [AtCoder Regular Contest 051 D - 長方形](https://atcoder.jp/contests/arc051/tasks/arc051_d) (ax+by)
- [AtCoder Beginner Contest 244 Ex - Linear Maximization](https://atcoder.jp/contests/abc244/tasks/abc244_h) (ax+by)
- [Yandex Contest Алгоритм 2022 K. Stepwise subsequence ](https://contest.yandex.com/contest/42710/problems/K) (有理数クラスでac)

## 参考

- [https://github.com/kth-competitive-programming/kactl/blob/main/content/data-structures/LineContainer.h](https://github.com/kth-competitive-programming/kactl/blob/main/content/data-structures/LineContainer.h)
- [https://maspypy.github.io/library/convex/cht.hpp](https://maspypy.github.io/library/convex/cht.hpp)