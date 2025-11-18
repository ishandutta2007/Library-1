---
title: 代数的構造ラッパー (Algebra)
documentation_of: ../../src/Math/Algebra.hpp
---

## 概要

`Algebra.hpp` は、ユーザーが定義した代数的構造を持つ構造体をラップし、統一的な演算子 (`+`, `-`, `*`) を提供するクラステンプレート `Algebra<M>` を提供する。

これにより、例えばBITやポテンシャル付きUnion-Find、行列ライブラリなど、特定の代数的構造をテンプレート引数として受け取るデータ構造に対して、自作の演算を簡単かつ統一的なインターフェースで渡すことが可能になる。

## 使い方

`Algebra<M>` を利用するには、代数的構造を定義した構造体 `M` をテンプレート引数として渡す。`M` は以下の仕様を満たす必要がある。

### 構造体 `M` の仕様

| メンバー | 説明 | 必須/任意 |
|:---|:---|:---:|
| `using T = ...;` | 演算対象となる型。 | 必須 |
| `static T o;` | 加法単位元（ゼロ）。`Algebra`クラスのデフォルトコンストラクタで使われる。 | 任意 |
| `static T i;` | 乗法単位元（イチ）。 | 任意 |
| `static T add(T a, T b);` | `a + b` の演算を定義する。 | `+`, `-` 演算子を使う場合に必須 |
| `static T neg(T a);` | `a` の加法逆元 `-a` を定義する。 | `-`, `-=` 演算子を使う場合に必須 |
| `static T mul(T a, T b);` | `a * b` の演算を定義する。 | `*`, `*=` 演算子を使う場合に必須 |

`o` や `i` が未定義の場合、`T` のデフォルトコンストラクタ (`T()`) の値が代わりに利用される。

---

## 実装例

### 例1: 整数の加法群

`int` の加法と減算のみを定義した例。

```cpp
#include "src/Math/Algebra.hpp"

struct AdditiveGroup {
  using T = int;
  static constexpr T o = 0; // 加法単位元
  static T add(T a, T b) { return a + b; }
  static T neg(T a) { return -a; }
};

using IntAdd = Algebra<AdditiveGroup>;
// IntAdd x = 5;
// IntAdd y = 3;
// auto z = x + y; // z.x == 8
// auto w = -x;    // w.x == -5
```

### 例2: Max-Plus 半環

`max` を加法、`+` を乗法とみなす半環。区間更新クエリなどで利用される。

```cpp
#include "src/Math/Algebra.hpp"
#include <algorithm>

struct MaxPlusSemiring {
  using T = int;
  static constexpr T o = -1e9; // 加法単位元 (max(a, -1e9) = a)
  static constexpr T i = 0;    // 乗法単位元 (a + 0 = a)
  static T add(T a, T b) { return std::max(a, b); }
  static T mul(T a, T b) { return a + b; }
};

using MaxPlus = Algebra<MaxPlusSemiring>;
// MaxPlus x = 5;
// MaxPlus y = 3;
// auto z = x + y; // z.x == 5 (max(5, 3))
// auto w = x * y; // w.x == 8 (5 + 3)
```

### 例3: アフィン変換群 (非可換)

`x` を `ax+b` に変換するアフィン変換。演算は非可換である。ポテンシャル付きUnion-Findなどで利用される。

```cpp
#include "src/Math/Algebra.hpp"
#include <utility>

struct AffineGroup {
  using T = std::pair<long long, long long>; // {a, b}
  static constexpr T o = {1, 0}; // 恒等変換 (1*x + 0)
  // (c, d) を適用した後に (a, b) を適用
  // a(cx+d)+b = acx + ad+b
  static T add(const T& f, const T& g) {
    auto [a, b] = f;
    auto [c, d] = g;
    return {a * c, a * d + b};
  }
  // 逆変換 (x-b)/a
  static T neg(const T& f) {
    auto [a, b] = f;
    // aの逆元が必要。ここでは簡単のため a=1 or -1 とする
    return {a, -b * a};
  }
};

using Affine = Algebra<AffineGroup>;
```

## Verify

- [AtCoder Beginner Contest 009 D - 漸化式](https://atcoder.jp/contests/abc009/tasks/abc009_4) (bitwise xor/and 半環を行列に乗せる)<br>
- [DISCO presents ディスカバリーチャンネル コードコンテスト2020 本戦 B - Hawker on Graph](https://atcoder.jp/contests/ddcc2020-final/tasks/ddcc2020_final_b) (maxと一次関数の合成を半環とみなす)<br>
- [競プロ典型 90 問　068 - Paired Information（★5）](https://atcoder.jp/contests/typical90/tasks/typical90_bp) (ポテンシャル付きUnion-Findに非可換なアフィン変換群を乗せる)<br>