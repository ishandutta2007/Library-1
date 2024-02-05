---
title: 約数配列
documentation_of: ../../src/NumberTheory/ArrayOnDivisors.hpp
---

添字が整数 $N$ の約数のみの配列.\
以下，$d(\cdot)$ を約数の個数を表す関数とする.

## メンバ関数

| 名前    | 概要  | 計算量    |
| --- | --- | --- |
| 1. `ArrayOnDivisors(N, f, d)` <br> 2. `ArrayOnDivisors(N, f)` <br> 3. `ArrayOnDivisors(N)` | コンストラクタ. <br> 1. 整数 $N$ とその[素因数分解](md/NumberTheory/Factors.md) $f$, 約数 $d$ を引数にする. <br> 2. 整数 $N$ とその素因数分解 $f$ を引数にする. <br> 3.  整数 $N$ を引数にする. | 1. $\mathcal{O}(d(N))$  <br> 2. $\mathcal{O}(d(N))$ <br> 3. $\mathcal{O}(N^{1/4})$  |
|`operator[](i)`| 添字 $i$ の値を左参照値で返す. <br> $i$ が $N$ の約数でないなら `assert` で落ちる. ||
|`size()`|配列のサイズを返す. つまり $d(N)$.||
|`begin()`|(添字，値)のペアの列のイテレータ.||
|`end()`|(添字，値)のペアの列のイテレータ.||
|1. `divisor_zeta()` <br> 2. `divisor_zeta(add)`|約数ゼータ変換 $\sum_{m\vert n} a(m)$ を行う. <br> 2. 関数`add(T&a,T b)` を与える. |64bitの$N$ で高々 $2^{17}$ 程度|
|1. `divisor_mobius()` <br> 2. `divisor_mobius(sub)`|約数メビウス変換 $\sum_{m\vert n}\mu(n/m)a(m)$ を行う. <br> 2. 関数`sub(T&a,T b)` を与える.|64bitの$N$ で高々 $2^{17}$ 程度|
|1. `multiple_zeta()` <br> 2. `multiple_zeta(add)`|倍数ゼータ変換 $\sum_{n\vert m} a(m)$ を行う. <br> 2. 関数`add(T&a,T b)` を与える.|64bitの$N$ で高々 $2^{17}$ 程度|
|1. `multiple_mobius()` <br> 2. `multiple_mobius(sub)`|倍数メビウス変換 $\sum_{n\vert m}\mu(m/n)a(m)$ を行う.  <br> 2. 関数`sub(T&a,T b)` を与える.|64bitの$N$ で高々 $2^{17}$ 程度|

## 参考
[https://maspypy.github.io/library/nt/array_on_divisors.hpp](https://maspypy.github.io/library/nt/array_on_divisors.hpp)