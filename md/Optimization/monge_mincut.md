---
title: 最小カット問題のk値への一般化
documentation_of: ../../src/Optimization/monge_mincut.hpp
---
phi関数等で ∞ を返すときはその大きさに注意 (大きすぎるとオーバーフロー)

## 関数

|名前|概要|
|---|---|
|`monge_mincut<MF>(n,k,theta,phi)`|$\begin{aligned} \min_{x\in\lbrace 0,\dots,k-1 \rbrace^n} \left(\sum_{i=0}^{n-1} \theta_i(x_i) + \sum_{i\lt j}\phi_{ij}(x_i,x_j)\right) \end{aligned}$ を求める(その時の解$x$も求める). <br> ただし $\phi_{ij}(\cdot,\cdot)$ はMonge ( 特に $\phi_{ij}(a,b)+\phi_{ij}(a+1,b+1)\leq \phi_{ij}(a,b+1)+\phi_{ij}(a+1,b)$ ) <br> テンプレート引数で最大流クラスを渡す. |

## 使用例
```cpp
 auto theta= [&](int i, int xi) -> long long {
  ...
 };
 auto phi= [&](int i, int j, int xi, int xj) -> long long {
  ...
 };
 using MF = MaxFlow<Dinic<long long>>;
 auto [ans, x] = monge_mincut<MF>(N, k, theta, phi);
```

## Verify

- [Kyoto University Programming Contest 2019 H - 123パズル](https://atcoder.jp/contests/kupc2019/tasks/kupc2019_h)
- [AtCoder Regular Contest 085 E - MUL](https://atcoder.jp/contests/arc085/tasks/arc085_c)

## 参考
[https://noshi91.hatenablog.com/entry/2021/06/29/044225](https://noshi91.hatenablog.com/entry/2021/06/29/044225)