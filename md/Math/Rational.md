---
title: 有理数 (Rational)
documentation_of: ../../src/Math/Rational.hpp
---

## 概要

`long long` や `__int128_t` などの整数型 `Int` を用いて有理数を扱うクラス。
四則演算、比較演算、各種ユーティリティ関数を提供する。
分母が `0` の場合は正負の無限大として扱われる。

## テンプレートパラメータ

|パラメータ|説明|デフォルト|
|---|---|---|
|`Int`|有理数の分子・分母を表現するための整数型。| |
|`reduction`|`true` の場合、コンストラクタや演算のたびに自動で約分を行う。`false` の場合は約分を行わない。|`true`|

---

## コンストラクタ

|形式|例|説明|
|---|---|---|
|`Rational()`|`Rational<long long> r;`|`0/1` で初期化する。|
|`Rational(num, den)`|`Rational<long long> r(3, 4);`|`num/den` で初期化する。`den` を省略した場合は `num/1` となる。|
|`Rational(str)`|`Rational<long long> r("3/4");`|文字列から `num/den` をパースして初期化する。|

---

## メンバ関数・フリー関数

|関数|概要|計算量|
|---|---|---|
|`to_fp()`|`long double` 型に変換する。`explicit operator` が定義されているため `(double)r` のようにキャスト可能。| $O(1)$ |
|`floor()`|床関数 $\lfloor r \rfloor$ を計算する。| $O(1)$ |
|`ceil()`|天井関数 $\lceil r \rceil$ を計算する。| $O(1)$ |
|`abs()`|絶対値 $|r|$ を計算する。| $O(1)$ |
|`to_string()`|`"num/den"` 形式の文字列に変換する。| $O(\log(\text{num}) + \log(\text{den}))$ |

---

## 演算子

- **四則演算**: `+`, `-`, `*`, `/`, `+=`, `-=`, `*=`, `/=`
- **比較演算**: `==`, `!=`, `<`, `>`, `<=`, `>=`
- **単項演算**: `-` (符号反転)

`reduction=true` の場合、乗算・除算ではクロス約分による最適化が行われる。

---

## 入出力

標準ストリームを用いた入出力に対応している。

```cpp
Rational<long long> r;
std::cin >> r; // "3/4" のような形式で入力
r *= 2;
std::cout << r << std::endl; // "3/2" と出力される
```

---

## 計算量

`g = binary_gcd(num, den)` の計算量を $O(\log(\max(|\text{num}|, |\text{den}|)))$ とする。

- **`reduction=true` (デフォルト)**
  - コンストラクタ: $O(g)$
  - 加算・減算: $O(g)$
  - 乗算・除算: $O(g)$ (クロス約分のため)
- **`reduction=false`**
  - すべての四則演算: $O(1)$

---

## Verify

- [CPSCO2019 Session3 G - Grand Election](https://atcoder.jp/contests/cpsco2019-s3/tasks/cpsco2019_s3_g)
- [Yandex Contest Алгоритм 2022 K. Stepwise subsequence](https://contest.yandex.com/contest/42710/problems/K) (CHT, doubleだとWA, `__int128_t` で `reduction=false` にしてAC)

```