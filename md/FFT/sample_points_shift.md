---
title: 多項式の評価点シフト
documentation_of: ../../src/FFT/sample_points_shift.hpp
---
次数 $n-1$ 以下の多項式 $f(x)$ について、$y_i = f(i) \ (i=0, 1, \dots, n-1)$ が与えられたとき、$f(c), f(c+1), \dots, f(c+m-1)$ を計算する。

## `sample_points_shift`

```cpp
template <class mod_t, size_t LM= 1 << 23>
std::vector<mod_t> sample_points_shift(const std::vector<mod_t>& y, mod_t c, int m);
```

|引数・返り値|概要|
|---|---|
|`const std::vector<mod_t>& y`|多項式 $f(x)$ の $x=0, 1, \dots, n-1$ での評価値 $y_0, y_1, \dots, y_{n-1}$ を格納した `vector`。$n$ は `y.size()`。|
|`mod_t c`|シフト先の開始点。|
|`int m`|求める評価点の個数。|
|`std::vector<mod_t>` (返り値)|$f(c), f(c+1), \dots, f(c+m-1)$ の値を格納した `vector`。|

### 制約
- `y.size() <= mod`
- `m <= mod`
- `mod_t` は `ModInt` のような剰余環のクラスであること。

### 計算量
$O((n+m)\log(n+m))$

### 使用例
```cpp
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/sample_points_shift.hpp"

int main() {
    using Mint = ModInt<998244353>;
    int n = 5, m = 5;
    Mint c = 10;
    // f(x) = x^2 + x + 1
    // f(0)=1, f(1)=3, f(2)=7, f(3)=13, f(4)=21
    std::vector<Mint> y = {1, 3, 7, 13, 21};

    // f(10), f(11), f(12), f(13), f(14) を求める
    auto res = sample_points_shift(y, c, m);

    for (int i = 0; i < m; ++i) {
        // f(10) = 111, f(11) = 133, ...
        std::cout << res[i] << std::endl;
    }
}
```
