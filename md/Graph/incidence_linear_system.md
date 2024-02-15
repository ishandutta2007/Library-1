---
title: 接続行列の連立方程式
documentation_of: ../../src/Graph/incidence_linear_system.hpp
---

|関数名|概要|計算量|
|---|---|---|
|`incidence_linear_system<T>(g, b)`|有向グラフ $g$ の接続行列 $A$ と頂点数 $n$ の次元を持つベクトル $\boldsymbol{b}$ に対して <br> $\displaystyle A\boldsymbol{x}=\boldsymbol{b}$<br>を満たす辺数 $m$ の次元を持つベクトル $\boldsymbol{x}$ を一つ返す. <br> 解なしなら空集合を返す.|$O(n+m)$|

## 問題例
[AtCoder Beginner Contest 155 F - Perils in Parallel](https://atcoder.jp/contests/abc155/tasks/abc155_f) (sp judge)\
[AtCoder Grand Contest 035 B - Even Degrees ](https://atcoder.jp/contests/agc035/tasks/agc035_b) (sp judge)\
[CODE FESTIVAL 2017 Final E - Combination Lock](https://atcoder.jp/contests/cf17-final/tasks/cf17_final_e)