---
title: 多変数畳み込み
documentation_of: ../../src/FFT/MultiVariateConvolution.hpp
---

## 概要

$k$ 変数の多項式 $F(x_0, \dots, x_{k-1}), G(x_0, \dots, x_{k-1})$ の積 $H = F \cdot G$、すなわち多次元配列の畳み込みを計算する。

$f, g$ をそれぞれ $F, G$ の係数を格納した $d_0 \times d_1 \times \dots \times d_{k-1}$ 次元の配列とする。
このとき、畳み込みの結果である配列 $h$ の各要素 $h_{i_0, \dots, i_{k-1}}$ は以下で定義される。

$$
h_{i_0, \dots, i_{k-1}} = \sum_{j_0=0}^{i_0} \dots \sum_{j_{k-1}=0}^{i_{k-1}} f_{j_0, \dots, j_{k-1}} g_{i_0-j_0, \dots, i_{k-1}-j_{k-1}}
$$

ライブラリの入力として与える配列は、多次元配列を1次元化したベクトルである。インデックスの対応はリトルエンディアン、すなわち多次元インデックス $(i_0, i_1, \dots, i_{k-1})$ は、1次元配列のインデックス $\sum_{j=0}^{k-1} i_j \left(\prod_{l=0}^{j-1} d_l\right)$ に対応する。

## 使い方

### コンストラクタ

`MultiVariateConvolution(const std::vector<int>& dim)`

- `dim`: 各次元の大きさ $d_0, d_1, \dots, d_{k-1}$ を格納した `std::vector<int>`。
- 計算量: $O(k \prod d_i)$

### メソッド

`convolve(const std::vector<mod_t>& f, const std::vector<mod_t>& g)`

- `f`, `g`: 畳み込みたい1次元化された配列 (`std::vector<mod_t>`)。サイズはともにコンストラクタに渡した次元の総積 $\prod d_i$ と等しくなければならない。
- `mod_t`: `ModInt` のようなNTT（数論変換）が可能な剰余環の型。
- 戻り値: 畳み込みの結果を `std::vector<mod_t>` として返す。
- 計算量: $N = \prod d_i$ として、$O(k N \log N)$

`size()`

- 配列の全要素数 $N = \prod d_i$ を返す。
- 計算量: $O(1)$

`dim()`

- 次元の数 $k$ を返す。
- 計算量: $O(1)$

## 使用例

```cpp
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/MultiVariateConvolution.hpp"

using Mint = ModInt<998244353>;

int main() {
    // 2変数、サイズ 2x2 の多項式 f(x,y) = 1+x, g(x,y) = 1+y の畳み込み
    // h(x,y) = (1+x)(1+y) = 1+x+y+xy
    std::vector<int> dim = {2, 2};
    MultiVariateConvolution mvc(dim);
    int n = mvc.size(); // n = 4

    // f の係数 (1, 1, 0, 0) -> 1 + 1*x + 0*y + 0*xy
    std::vector<Mint> f(n);
    f[0] = 1; // 1
    f[1] = 1; // x

    // g の係数 (1, 0, 1, 0) -> 1 + 0*x + 1*y + 0*xy
    std::vector<Mint> g(n);
    g[0] = 1; // 1
    g[2] = 1; // y

    auto h = mvc.convolve(f, g);

    // h の係数は (1, 1, 1, 1) -> 1 + 1*x + 1*y + 1*xy になるはず
    for (int i = 0; i < n; ++i) {
        std::cout << h[i] << (i == n - 1 ? "" : " ");
    }
    std::cout << std::endl;

    return 0;
}
```

## 制約
- `convolve` のテンプレート引数 `mod_t` は、NTTが可能な法を持つ `ModInt` である必要があります。

## 参考
- [https://37zigen.com/truncated-multivariate-convolution/](https://37zigen.com/truncated-multivariate-convolution/)
