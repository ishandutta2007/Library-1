---
title: 約数配列
documentation_of: ../../src/NumberTheory/ArrayOnDivisors.hpp
---


## `ArrayOnDivisors<Int, T>` クラス

添字が整数 $N$ の約数のみの配列.\
以下，$d(\cdot)$ を約数の個数を表す関数とする.

| メンバ変数  | 概要  |
| --- | --- | 
|`factors` | $N$ の[素因数分解](Factors.hpp) |

| メンバ関数  | 概要  | 計算量    |
| --- | --- | --- |
| 1. `ArrayOnDivisors(N, f, d)` <br> 2. `ArrayOnDivisors(N, f)` <br> 3. `ArrayOnDivisors(N)` | コンストラクタ. <br> 1. 整数 $N$ とその[素因数分解](Factors.hpp), $N$ の全約数の列 を引数にする. <br> 2. 整数 $N$ とその素因数分解 を引数にする. <br> 3.  整数 $N$ を引数にする. | 1. $O(d(N))$  <br> 2. $O(d(N))$ <br> 3. $O(N^{1/4})$  |
|`operator[](i)`| 添字 $i$ の値を左参照値で返す. <br> $i$ が $N$ の約数でないなら `assert` で落ちる. ||
|`size()`|配列のサイズを返す. つまり $d(N)$.||
|`begin()`|(添字，値)のペアの列のイテレータ.||
|`end()`|(添字，値)のペアの列のイテレータ.||
|1. `divisor_zeta()` <br> 2. `divisor_zeta(add)`|約数ゼータ変換 $\sum_{m\vert n} a(m)$ を行う. <br> 2. 関数`add(T&a,T b)` を与える. | $O(d(N))$ |
|1. `divisor_mobius()` <br> 2. `divisor_mobius(sub)`|約数メビウス変換 $\sum_{m\vert n}\mu(n/m)a(m)$ を行う. <br> 2. 関数`sub(T&a,T b)` を与える.| $O(d(N))$ |
|1. `multiple_zeta()` <br> 2. `multiple_zeta(add)`|倍数ゼータ変換 $\sum_{n\vert m} a(m)$ を行う. <br> 2. 関数`add(T&a,T b)` を与える.| $O(d(N))$ |
|1. `multiple_mobius()` <br> 2. `multiple_mobius(sub)`|倍数メビウス変換 $\sum_{n\vert m}\mu(m/n)a(m)$ を行う.  <br> 2. 関数`sub(T&a,T b)` を与える.| $O(d(N))$ |
|`set_multiplicative(f)`|乗法的関数 $f$ によってテーブルを埋める. <br> 素冪の場合 $f(p^e)$ を表す `f(p,e)` を渡す. |$O(d(N))$|
|`set_totient(f)`|オイラーのトーシェント関数 $\varphi$ によってテーブルを埋める. |$O(d(N))$|
|`set_mobius(f)`|メビウス関数 $\mu$ によってテーブルを埋める. |$O(d(N))$|

## Verify
- [AtCoder Regular Contest 064 F - Rotated Palindromes](https://atcoder.jp/contests/arc064/tasks/arc064_d) (約数メビウス)

## 参考
[https://maspypy.github.io/library/nt/array_on_divisors.hpp](https://maspypy.github.io/library/nt/array_on_divisors.hpp)