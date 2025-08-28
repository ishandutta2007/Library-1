---
title: Binary GCD
documentation_of: ../../src/Math/binary_gcd.hpp
---

除算の代わりにシフト演算と引き算を使うことで高速に最大公約数を求めるアルゴリズム。
`constexpr` での呼び出しが可能。

|名前|概要|
|---|---|
|`binary_gcd(a,b)`| 整数 $a,b$ の最大公約数を返す。|

## 計算量
$O(\log(\min(|a|,|b|)))$

## 備考
- 引数 $a, b$ が負数の場合でも、`gcd(|a|, |b|)` と等価な結果を返す。
