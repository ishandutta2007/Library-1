---
title: 最小多項式 (Minimal Polynomial)
documentation_of: ../../src/LinearAlgebra/MinimalPolynomial.hpp
---

## 概要
ベクトル $\boldsymbol{b}$ と、それを定義域・値域に持つ線形写像 $M$ が与えられたとき、$p(M)\boldsymbol{b}=\boldsymbol{0}$ を満たす最小次数のモニック多項式 $p(x)$ を $\boldsymbol{b}$ の $M$ に関する最小多項式という。
このライブラリは、最小多項式を求めることで、$M^k \boldsymbol{b}$ のような線形写像のべき乗とベクトルの積を高速に計算する機能を提供する。

`ModInt` 系（`is_modint_v<T>` が `true` となる型）のテンプレート `mod_t` を使用する。
線形写像 $M$ は、`Vector<mod_t>` を引数に取り `Vector<mod_t>` を返す `operator()` を持つ任意の型 (`Matrix` クラスやラムダ式など) で与えることができる。

## `MinimalPolynomial` クラス

### メンバ関数

| 関数 | 概要 | 計算量 |
| --- | --- | --- |
| `MinimalPolynomial(M,b)` | コンストラクタ。線形写像 `M` とベクトル `b` を与えて最小多項式を求める。`M` は $n$ 次元の線形写像。 | $O(d \cdot T(n) + d^2)$ <br> $T(n)$ は $M$ とベクトルの積の計算量、 $d$ は求める多項式の次数 ($d \le n$) |
| `degree()` | 最小多項式の次数を返す。 | $O(1)$ |
| `operator[](i)` | 最小多項式の $x^i$ の係数を返す。 | $O(1)$ |
| `pow(k)` | $M^k\boldsymbol{b}$ を `Vector` として返す。 | $O(d^2 \log k + dn)$ <br> $d$ は最小多項式の次数 |

## `linear_map_det` 関数
| 関数 | 概要 | 計算量 |
| --- | --- | --- |
| `linear_map_det<mod_t>(M,n)` | $n \times n$ 行列で表現される線形写像 $M$ の行列式 $\det M$ を返す。**Schwartz-Zippel の補題に基づく確率的アルゴリズムであり、ごく稀に誤った値を返す可能性がある。** | $O(n \cdot T(n) + n^2)$ <br> $T(n)$ は $M$ とベクトルの積の計算量 |

## 使用例

```cpp
// 3x3 行列の 100 乗とベクトルの積を計算
using Mint = ModInt<998244353>;
Matrix<Mint> A(3, 3);
A[0][0]=1; A[0][1]=1; A[0][2]=0;
A[1][0]=0; A[1][1]=1; A[1][2]=1;
A[2][0]=1; A[2][1]=0; A[2][2]=1;

Vector<Mint> b = {1, 0, 0};

// A を線形写像として MinimalPolynomial を構築
MinimalPolynomial mp(A, b);

// A^100 * b を計算
Vector<Mint> res = mp.pow(100);


// 疎行列の行列式を計算
// auto f = [&](const Vector<Mint>& v) { ... };
// linear_map_det<Mint>(f, N);
```

## 参考
- [Black Box Linear Algebra - yukicoder Wiki](https://yukicoder.me/wiki/black_box_linear_algebra)