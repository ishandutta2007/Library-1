---
title: Pointwise
documentation_of: ../../src/Misc/Pointwise.hpp
---

## 概要

`Pointwise<K1, K2, ..., Kn>` は、複数の型 `K1, K2, ..., Kn` の値をまとめたタプルに対し、要素ごとの演算を定義するクラステンプレートである。

例えば、`Pointwise<Mint, Mint>` は2つの `Mint` 型の値をペアで保持し、`+`, `-`, `*`, `/` などの二項演算子をペアの各要素に対して同時に適用する。これにより、複数のハッシュ基数を用いたローリングハッシュの実装などを簡潔に記述できる。

## 使い方

### ローリングハッシュでの利用例

2つの異なる法と基数でローリングハッシュを計算する例。

```cpp
#include "src/Math/ModInt.hpp"
#include "src/Misc/Pointwise.hpp"
#include "src/String/RollingHash.hpp"

// 2つのModIntを束ねる
using Mint1 = ModInt<998244353>;
using Mint2 = ModInt<1000000007>;
using K = Pointwise<Mint1, Mint2>;

// Kを値の型としてローリングハッシュを定義
using RH = RollingHash<K>;

// 2つのハッシュの基数をそれぞれ初期化
RH::init({base1, base2});

std::string s = "abcabc";
RH rh(s);

// ハッシュ値は Pointwise<Mint1, Mint2> 型で得られる
auto hash_value = rh.get(0, 3); // s[0..3) のハッシュ値
```

## 提供される機能

`Pointwise<Ks...>` は `std::tuple<Ks...>` を継承している。

### 演算子

`Pointwise` 型のオブジェクト `a`, `b` に対し、以下の演算子が定義されている。演算は `std::tuple` の各要素に対して行われる。

- `a + b`
- `a - b`
- `a * b`
- `a / b`
- `a += b`
- `a -= b`
- `a *= b`
- `a /= b`

### コンストラクタ

- `Pointwise(v1, v2, ...)`: 各要素を個別に初期化する。
- `Pointwise(v)`: 全ての要素を単一の値 `v` で初期化する。

## Verify
- [AtCoder Regular Contest 099 F - Eating Symbols Hard](https://atcoder.jp/contests/arc099/tasks/arc099_d)(多項式ハッシュ)
- [第二回全国統一プログラミング王決定戦本戦 D - 木、](https://atcoder.jp/contests/nikkei2019-2-final/tasks/nikkei2019_2_final_d) (根付き木ハッシュ)