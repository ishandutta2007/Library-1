---
title: 接続行列の連立方程式
documentation_of: ../../src/Graph/incidence_matrix_equation.hpp
---

|関数名|概要|計算量|
|---|---|---|
|`incidence_matrix_equation<T>(g, b)`|有向グラフ $g$ の接続行列 $A$ と頂点数 $n$ の次元を持つベクトル $\boldsymbol{b}$ に対して <br> $\displaystyle A\boldsymbol{x}=\boldsymbol{b}$<br>を満たす辺数 $m$ の次元を持つベクトル $\boldsymbol{x}$ を一つ返す. <br> 引数は [`Graph`クラス](src/Graph/Graph.hpp) と `vector<T>`. <br> 戻り値は `vector<T>` . <br> 解なしなら空集合を返す. <br>また `T=bool` の場合は有限体 $\mathbb{F}_2$ だとみなす.|$O(n+m)$|

## Verify


- [AtCoder Beginner Contest 155 F - Perils in Parallel](https://atcoder.jp/contests/abc155/tasks/abc155_f) (sp judge)<br>
- [AtCoder Grand Contest 035 B - Even Degrees ](https://atcoder.jp/contests/agc035/tasks/agc035_b) (sp judge)

