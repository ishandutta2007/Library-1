---
title: 素数の列挙
documentation_of: ../../src/NumberTheory/enumerate_primes.hpp
---
線形篩による素数列挙の前処理 

## 関数

|名前|概要|計算量|
|---|---|---|
| `enumerate_primes(N)`| $N$ 以下のすべての素数を返す. <br> 返り値は [`ListRange<int>`](src/Internal/ListRange.hpp)|　$\mathcal{O}(N)$|
|`least_primes_factor(n)`| $n$ の最小素因数を返す. ||
|`completely_multiplicative_table<T>(N, f)`| 完全乗法的関数 $f$ の値が入ったテーブルを返す. <br> $f$ は素冪のもの `f(p,e)`$=f(p^e)$ を渡す(完全だけど).<br> サイズは $N+1$. |$\displaystyle\mathcal{O}\left(N + \frac{N\alpha}{\log N}\right)$ <br> ただし $f(p)$ の計算量を $\mathcal{O}(\alpha)$ とおいた.|
|`pow_table<T>(N,k)`| $n^k$ の値が入ったテーブルを返す. <br> サイズは $N+1$. |$\displaystyle\mathcal{O}\left(N + \frac{N\log k}{\log N}\right)$|

## 参考
[https://37zigen.com/linear-sieve/](https://37zigen.com/linear-sieve/) 
