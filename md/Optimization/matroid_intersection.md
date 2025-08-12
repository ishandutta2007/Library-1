---
title: マトロイド交叉
documentation_of: ../../src/Optimization/matroid_intersection.hpp
---

$n$は台集合のサイズ, $r$は最大共通独立集合のサイズ, $\tau$は独立性オラクル呼び出しにかかる時間

## 重みなし

| 関数名                          | 概要                                                                                | 計算量                      |
| ------------------------------- | ----------------------------------------------------------------------------------- | --------------------------- |
| `matroid_intersection(n,M1,M2)` | (引数) `n` : 台集合のサイズ, `M1`・`M2` : マトロイド <br> (返り値) 最大共通独立集合 | $O(nr^{1.5}\tau)$ |

## 重みあり

| 関数名                                          | 概要                                                                                                                                                                                                  | 計算量                  |
| ----------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------- |
| `weighted_matroid_intersection<sgn>(n,M1,M2,w)` | (引数) `n` : 台集合のサイズ, `M1`・`M2` : マトロイド, `w` : 重み <br> (返り値) 共通独立集合の集合 ( k番目の集合は 要素数 = k かつ 重みが最大(最小)のもの )　<br> template引数で最大最小を指定する | $O(nr^2\tau)$ |


## 参考
[https://hitonanode.github.io/cplib-cpp/combinatorial_opt/matroid_intersection.hpp](https://hitonanode.github.io/cplib-cpp/combinatorial_opt/matroid_intersection.hpp) \
William H. Cunningham. Improved bounds for matroid partition and intersection algorithms. SIAMJournal on Computing (SICOMP), 15(4):948–957, 1986.

## Verify

- [beecrowd | 2128 Demonstration of Honesty!](https://www.beecrowd.com.br/judge/en/problems/view/2128) (グラフ+分割) 
- [Hello 2020 G. Seollal - Codeforces](https://codeforces.com/contest/1284/problem/G) (グラフ+分割)
- [2019 Petrozavodsk Winter Camp, Yandex Cup D. Pick Your Own Nim - Codeforces](https://codeforces.com/gym/102156/problem/D) (二値+分割)
- [幾何コンテスト2013 B - 玉座の間](https://atcoder.jp/contests/geocon2013/tasks/geocon2013_b) (重み二部マッチング, 重みが実数) 
- [ACM-ICPC Japan Alumni Group Summer Camp 2012 Day 2 まるかいて](https://onlinejudge.u-aizu.ac.jp/problems/2429) (重み二部マッチング, |台集合| <= 1e4, 構築, sp judge) 
- [AtCoder Library Practice Contest E - MinCostFlow](https://atcoder.jp/contests/practice2/tasks/practice2_e) (重み付き, 分割+分割) 
- [Deltix Round, Summer 2021 H. DIY Tree - Codeforces](https://codeforces.com/contest/1556/problem/H) (重み付き, グラフ+分割) 
- [NAIPC 2018 G Rainbow Graph](https://www.acmicpc.net/problem/16046)(重み付き, あるマトロイド (グラフマトロイドの双対マトロイドと自由マトロイドの直和?) 同士の交差)