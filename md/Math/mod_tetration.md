---
title: テトレーション $a\upuparrows b$ ($\mathbb{Z}/m\mathbb{Z}$)
documentation_of: ../../src/Math/mod_tetration.hpp
---

## 概要
$a$ の $b$ 回のテトレーションを $m$ で割った余りを計算する。
$\display a \upuparrows b = \underbrace{a^{a^{\dots^a}}}_{b個} \pmod m $$

`mod_tetration(a, b, m)` はこの値を返す。

## 使い方
```cpp
uint64_t mod_tetration(uint64_t a, uint64_t b, uint64_t m);
```

## 制約
- `1 <= m`
- `a, b` は `uint64_t` の範囲の値
- `constexpr` での計算に対応している。

## アルゴリズム
オイラーの定理を拡張した $x^y \equiv x^{y \pmod{\phi(m)} + \phi(m)} \pmod m$ (ただし $y \ge \phi(m)$ ) を利用する。
指数部分である $a \upuparrows (b-1)$ について、 $\pmod{\phi(m)}$ での値を再帰的に計算することで、全体の値を求める。
法である $m$ は $\phi(m), \phi(\phi(m)), \dots$ と小さくなっていき、最終的に 1 になることを利用している。

## 計算量
$O(m^{1/4})$
(法 $m$ の素因数分解の計算量がボトルネックとなる)

## 使用例
```cpp
#include "src/Math/mod_tetration.hpp"
#include <iostream>

int main() {
    // 2^2^2 mod 1000000007
    constexpr auto f = mod_tetration(2, 3, 1000000007);
    static_assert(f == 16);
    std::cout << f << std::endl; // 16

    // 3^3^3 mod 100
    // 3^27 mod 100
    auto g = mod_tetration(3, 3, 100);
    std::cout << g << std::endl; // 87
    return 0;
}
```

## Verify
- [Summer Festival Contest 2018 (Division 1) F - 冪冪ゲーム (Powerful Fever!)](https://atcoder.jp/contests/summerfes2018-div1/tasks/summerfes2018_f)