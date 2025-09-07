---
title: 疎な形式的冪級数
documentation_of: ../../src/Math/sparse_fps.hpp
---

## 概要

形式的冪級数 $f(x)$ のうち、非ゼロの係数を持つ項が少ないものを「疎な形式的冪級数」と呼びます。
このライブラリは、疎な形式的冪級数 $f(x)$ （または $g(x)$）を入力として、各種演算を高速に計算します。

入力の形式的冪級数は、`std::vector<mod_t>` の形式で与えます。これは密な表現ですが、内部で非ゼロ項を抽出して計算するため、計算量は非ゼロ項の数 $S$ に依存します。

すべての関数は `sfps` 名前空間に定義されています。

## 依存
- `src/Math/FactorialPrecalculation.hpp`
- `src/Math/mod_sqrt.hpp`

## 関数

|名前|概要|計算量|
|---|---|---|
|`inv(f, N)`|$1/f$ を $x^N$ の項まで計算する。 <br> $f$ の定数項が 0 の場合 `assert` で失敗する。 <br> 返り値のサイズは $N+1$。 |以下、$f, g$ の非ゼロ項数をそれぞれ $S_f, S_g$ とする。<br> $O(NS_f)$|
|`div(f, g, N)`| $f/g$ を $x^N$ の項まで計算する。 <br> $g$ の定数項が 0 の場合 `assert` で失敗する。 <br> $f$ は疎でなくともよい。　<br> 返り値のサイズは $N+1$。 | $O(NS_g)$|
|`log(f, N)`| $\log f$ を $x^N$ の項まで計算する。 <br> $f$ の定数項が 1 でない場合 `assert` で失敗する。  <br> 返り値のサイズは $N+1$。 | $O(NS_f)$|
|`exp(f, N)`| $\exp f$ を $x^N$ の項まで計算する。 <br> $f$ の定数項が 0 でない場合 `assert` で失敗する。  <br> 返り値のサイズは $N+1$。 | $O(NS_f)$|
|`pow(f, k, N)`| $f^k$ を $x^N$ の項まで計算する。<br> $k$ は `int64_t` 型であり、負の整数でもよい。 <br> $f(x)=c x^d + \\\dots$ ($c \\neq 0$) のとき、$d>0$ かつ $k<0$ の場合は不定のため `assert` で失敗する。<br> 返り値のサイズは $N+1$。 | $O(NS_f)$|
|`sqrt(f, N)`| $\sqrt{f}$ のひとつを $x^N$ の項まで計算する。　<br> 平方根が存在しない場合、空の `vector` を返す。 <br> 正常な返り値のサイズは $N+1$。 | $O(NS_f)$|
|`exp_of_div(f, g, N)`| $\exp (f/g)$ を $x^N$ の項まで計算する。 <br> $f/g$ の定数項が 0 でない場合 `assert` で失敗する。 <br> 返り値のサイズは $N+1$。 | $O(N(S_f+S_g))$|
|`pow_of_div(f, g, k, N)`| $(f/g)^k$ を $x^N$ の項まで計算する。 <br> $k$ は `int64_t` 型であり、負の整数でもよい。<br> 負冪の項を含むようなら `assert` で失敗する。 <br> 返り値のサイズは $N+1$。 | $O(N(S_f+S_g))$ |
|`pow_mul_pow(f, k, g, l, N)`| $f^kg^l$ を $x^N$ の項まで計算する。<br> $k,l$ は `int64_t` 型であり、負の整数でもよい。 <br> 負冪の項を含むようなら `assert` で失敗する。 <br> 返り値のサイズは $N+1$。 | $O(N(S_f+S_g))$|
|`sqrt_of_div(f, g, N)`| $\sqrt{f/g}$ を $x^N$ の項まで計算する。 <br> 平方根が存在しない場合、空の `vector` を返す。<br> 負冪の項を含むようなら `assert` で失敗する。 <br> 正常な返り値のサイズは $N+1$。 | $O(N(S_f+S_g))$|

## 使用例

### `inv`
$f(x) = 1 - x - x^2$ の逆数を $x^4$ まで求める。
答えは $1 + x + 2x^2 + 3x^3 + 5x^4$ （フィボナッチ数列）となる。

```cpp
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/sparse_fps.hpp"

using Mint = ModInt<998244353>;

int main() {
    // f(x) = 1 - x - x^2
    std::vector<Mint> f = {1, -1, -1};
    
    // 1/f を x^4 の項まで計算
    int N = 4;
    auto inv_f = sfps::inv(f, N);
    
    // 結果: 1 1 2 3 5
    for (int i = 0; i <= N; ++i) {
        std::cout << inv_f[i] << (i == N ? "" : " ");
    }
    std::cout << std::endl;
    
    return 0;
}
```

## 参考
[https://maspypy.com/多項式・形式的べき級数-高速に計算できるもの](https://maspypy.com/%E5%A4%9A%E9%A0%85%E5%BC%8F%E3%83%BB%E5%BD%A2%E5%BC%8F%E7%9A%84%E3%81%B9%E3%81%8D%E7%B4%9A%E6%95%B0-%E9%AB%98%E9%80%9F%E3%81%AB%E8%A8%88%E7%AE%97%E3%81%A7%E3%81%8D%E3%82%8B%E3%82%82%E3%81%AE)
