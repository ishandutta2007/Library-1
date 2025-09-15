---
title: 形式的冪級数の逆元
documentation_of: ../../src/FFT/fps_inv.hpp
---

## 概要
形式的冪級数 $A(x) = \sum_{i=0}^{N-1} a_i x^i$ の逆元 $B(x) = A(x)^{-1}$ を $\pmod{x^N}$ で求める。
すなわち、$A(x)B(x) \equiv 1 \pmod{x^N}$ となる $B(x) = \sum_{i=0}^{N-1} b_i x^i$ を計算する。

係数の型 `mod_t` は、体が想定されている（例: `ModInt`）。

計算量: $O(N \log N)$

## 制約
- $p$ は空でない (`p.size() > 0`)
- $p$ の定数項 `p[0]` は逆元を持つ必要がある

## 使用方法
```cpp
#include <vector>
#include "src/Math/ModInt.hpp" // ModIntなど
#include "src/FFT/fps_inv.hpp"
```

### inv(p)
- `p`: `vector<mod_t>` 型の形式的冪級数の係数ベクトル
- 戻り値: `p` の逆元の係数ベクトル (サイズは `p` と同じ)

## 使用例

```cpp
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/fps_inv.hpp"

// https://judge.yosupo.jp/problem/inv_of_formal_power_series
int main() {
    std::cin.tie(0);
    std::ios::sync_with_stdio(0);
    int N;
    std::cin >> N;
    using Mint = ModInt<998244353>;
    std::vector<Mint> a(N);
    for (int i = 0; i < N; i++) {
        std::cin >> a[i];
    }
    auto b = inv(a);
    for (int i = 0; i < N; i++) {
        std::cout << b[i] << (i == N - 1 ? "" : " ");
    }
    std::cout << std::endl;
    return 0;
}
```
