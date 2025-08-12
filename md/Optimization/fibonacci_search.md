---
title: フィボナッチ探索
documentation_of: ../../src/Optimization/fibonacci_search.hpp
---

引数の型が整数な単峰関数を対象. \
引数の型が `long double` の単峰関数を対象にする場合は[黄金分割探索](src/Optimization/golden_search.hppd) を使う.

## 関数

| 名前         | 概要                                                 | 計算量                         |
| ------------ | ---------------------------------------------------- | ------------------------------ |
| `fibonacci_search<sgn>(f,l,r)`   | $\lbrack l, r\rbrack \cap\mathbb{Z}$ 上で単峰となる関数 $f(x)$ の最適値とその最適解を返す. <br> 返り値は { 最適解 $x^\ast$, 最適値 $f(x^\ast)$ } <br> templateの引数で最大最小を指定.           | $n=r-l$ とおき, $f(x)$ の評価が$O(A)$ かかるとしたとき<br>                  $O(A\log n)$    |


## Verify

- [京セラプログラミングコンテスト2023(AtCoder Beginner Contest 305) Ex - Shojin](https://atcoder.jp/contests/abc305/tasks/abc305_h) (Alien DP)
- [技術室奥プログラミングコンテスト#2 F - NPCの家 (NPC's House)](https://atcoder.jp/contests/tkppc2/tasks/tkppc2016_f) (Alien DP)

## 参考
[https://qiita.com/tanaka-a/items/f380257328da421c6584](https://qiita.com/tanaka-a/items/f380257328da421c6584) 