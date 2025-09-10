---
title: exp, log, pow (形式的冪級数)
documentation_of: ../../src/FFT/fps_exp.hpp
---

## 概要
形式的冪級数 $f(x)$ に対して、以下の関数を計算する。

- $\exp(f(x)) = \sum_{i=0}^{\infty} \frac{f(x)^i}{i!}$
- $\log(f(x)) = \int f'(x) f(x)^{-1} dx$
- $\mathrm{pow}(f(x), k) = f(x)^k$

内部でニュートン法を用いており、`exp` と `log` は互いに逆関数の関係にある。

## 機能

### `exp(f)`
`exp(f(x))` を計算する。

#### シグネチャ
```cpp
template <class mod_t, size_t LM= 1 << 22>
vector<mod_t> exp(const vector<mod_t> &p);
```

#### 制約
- `f[0] == 0`

### `log(f)`
`log(f(x))` を計算する。

#### シグネチャ
```cpp
template <class mod_t, size_t LM= 1 << 22>
vector<mod_t> log(const vector<mod_t> &p);
```

#### 制約
- `f[0] == 1`

### `pow(f, k)`
`f(x)^k` を計算する。

$f(x) = c \cdot x^d \cdot g(x)$ (ただし $g(0)=1$) とすると、$f(x)^k = c^k \cdot x^{kd} \cdot g(x)^k = c^k \cdot x^{kd} \cdot \exp(k \log(g(x)))$ によって計算される。

#### シグネチャ
```cpp
template <class mod_t, size_t LM= 1 << 22>
vector<mod_t> pow(const vector<mod_t> &p, uint64_t k);
```

### `deriv(f)`
$f(x)$ の導関数 $f'(x)$ を計算する。

#### シグネチャ
```cpp
template <class mod_t>
vector<mod_t> deriv(const vector<mod_t> &p);
```

### `integ(f)`
$f(x)$ の不定積分 $\int f(x) dx$ を計算する。(積分定数は $0$)

#### シグネチャ
```cpp
template <class mod_t, size_t LM= 1 << 22>
vector<mod_t> integ(const vector<mod_t> &p);
```

## 計算量
$N$ を形式的冪級数の長さとする。
- `exp(f)`, `log(f)`, `pow(f, k)`: $O(N \log N)$
- `deriv(f)`, `integ(f)`: $O(N)$

## 使用例

```cpp
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/fps_exp.hpp"

int main() {
    using Mint = ModInt<998244353>;
    int n = 5;
    // f(x) = x + x^2/2! + x^3/3! + x^4/4!
    // exp(f(x)) = 1 + x + x^2 + x^3 + x^4 (mod x^5)
    std::vector<Mint> f(n);
    f[1] = 1;
    f[2] = Mint(1) / 2;
    f[3] = Mint(1) / 6;
    f[4] = Mint(1) / 24;

    auto g = exp(f);

    for (int i = 0; i < n; ++i) {
        std::cout << g[i] << (i == n - 1 ? "" : " ");
    }
    std::cout << std::endl;
    // 出力: 1 1 1 1 1
}
```
