---
title: 多点評価と多項式補間 (SubProductTree)
documentation_of: ../../src/FFT/SubProductTree.hpp
---

## 概要
多項式の多点評価 (Multipoint Evaluation) と、点の集合からの多項式補間 (Polynomial Interpolation) を高速に行う。

$N$ 個の点 $x_0, x_1, \dots, x_{N-1}$ が与えられたとき、
- **多点評価**: $N-1$ 次多項式 $f(X)$ に対し、$y_i = f(x_i)$ for $i=0, \dots, N-1$ をまとめて計算する。
- **多項式補間**: $N$ 個の点の集合 $(x_0, y_0), \dots, (x_{N-1}, y_{N-1})$ をすべて通る唯一の $N-1$ 次多項式 $f(X)$ を求める。

## 使い方
`SubProductTree<mod_t>` の形で使用する。

### コンストラクタ
`SubProductTree(const std::vector<mod_t> &xs)`
評価点または補間点の x 座標のリスト `xs` を渡して初期化する。

### メンバ関数
`std::vector<mod_t> multi_eval(const poly &f) const`
多項式 `f` (係数の `std::vector`) を、コンストラクタで指定した各点 $x_i$ で評価した値のリスト $\{f(x_0), f(x_1), \dots, f(x_{N-1})\}$ を返す。

`poly interpolate(const std::vector<mod_t> &ys)`
コンストラクタで指定した各点 $x_i$ と、それに対応する y 座標のリスト `ys` から、補間多項式の係数リストを返す。すなわち、すべての $i$ について $f(x_i) = y_i$ を満たす $N-1$ 次多項式 $f(X)$ を求める。

## 計算量
- コンストラクタ (前計算): $O(N (\log N)^2)$
- `multi_eval`: $O(N (\log N)^2)$
- `interpolate`: $O(N (\log N)^2)$

ここで $N$ は点の数。

## コード例
```cpp
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/SubProductTree.hpp"

void multipoint_evaluation_example() {
    using Mint = ModInt<998244353>;
    
    // 多項式 f(x) = 1 + 2x + 3x^2
    std::vector<Mint> f = {1, 2, 3};
    
    // 評価点 x = 0, 1, 2, 3, 4
    std::vector<Mint> xs = {0, 1, 2, 3, 4};
    
    SubProductTree<Mint> spt(xs);
    auto ys = spt.multi_eval(f);
    
    // f(0) = 1
    // f(1) = 6
    // f(2) = 17
    // f(3) = 34
    // f(4) = 57
    for (size_t i = 0; i < ys.size(); ++i) {
        std::cout << "f(" << xs[i] << ") = " << ys[i] << std::endl;
    }
}

void polynomial_interpolation_example() {
    using Mint = ModInt<998244353>;

    // 点 (0, 1), (1, 6), (2, 17) を通る多項式を求める
    // 答えは f(x) = 1 + 2x + 3x^2 のはず
    std::vector<Mint> xs = {0, 1, 2};
    std::vector<Mint> ys = {1, 6, 17};

    SubProductTree<Mint> spt(xs);
    auto f = spt.interpolate(ys);

    // f = {1, 2, 3}
    for (size_t i = 0; i < f.size(); ++i) {
        std::cout << "f[" << i << "] = " << f[i] << std::endl;
    }
}
```

## Verify
- [yosupo judge - Multipoint Evaluation](https://judge.yosupo.jp/problem/multipoint_evaluation)
- [yosupo judge - Polynomial Interpolation](https://judge.yosupo.jp/problem/polynomial_interpolation)
- [AtCoder Regular Contest 033 D - 見たことのない多項式](https://atcoder.jp/contests/arc033/tasks/arc033_4) (オーバーキル)