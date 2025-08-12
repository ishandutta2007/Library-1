---
title: Berlekamp-Massey
documentation_of: ../../src/Math/berlekamp_massey.hpp
---
数列$\lbrace a_i\rbrace$の最初の $n$ 項 $\lbrace a_i\rbrace_{i=0}^{n-1}$から、その数列を生成する最小の線形漸化式
$$
a_i = c_0 a_{i-1} + c_1 a_{i-2} + \cdots + c_{d-1}a_{i-d}
$$
を求める. ( $d\le n/2$ )

## 計算量
 $O(n^2) $

## Verify


- [天下一プログラマーコンテスト2015予選B C - 擬二等辺三角形](https://atcoder.jp/contests/tenka1-2015-qualb/tasks/tenka1_2015_qualB_c)


