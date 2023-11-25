---
title: 有名な数論的関数
documentation_of: ../../src/NumberTheory/famous_arithmetic_functions.hpp
---
有名な数論的関数についての補助的なツール.

名前空間 `famous_arithmetic_functions` に置いている.
```c++
using namespace famous_arithmetic_functions;
```
その中にさらに名前空間 `add`・`mul` を配置しており, それぞれ加法的関数・乗法的関数を置いている.

数論的関数 $f(n)$ について, この補助的クラスは `static` な関数 `f(p,e)` と `poly()` をもっている.

|||
|---|---|
|`f(p,e)`| $f(p^e)$ を表す関数. |
|`poly()`| $f(p)$ の $p$ の多項式としての係数. 返り値は `vector`.|

## 名前空間 `add` 内

|名前|概要|$f(p^e)$|OEIS|
|---|---|---|---|
|`BigOmega<T>`| $n$ の重複を含めた素因数の個数を表す関数 $\Omega(n)$ |　$e$ |[A001222](https://oeis.org/A001222)|
|`LittleOmega<T>`| $n$ の相異なる素因数の個数を表す関数 $\omega(n)$ |　$1$ |[A001221](https://oeis.org/A001221)|
|`Sopfr<T>`| $n$ の重複を含めた素因数の和を表す関数 $\mathrm{sopfr}(n)$ |　$p\cdot e$ |[A001414](https://oeis.org/A001414)|
|`Sopf<T>`| $n$ の相異なる素因数の和を表す関数 $\mathrm{sopf}(n)$ |　$p$ |[A008472](https://oeis.org/A008472)|

## 名前空間 `mul` 内

|名前|概要|$f(p^e)$|
|---|---|---|
|`Totient<T>`| [オイラーのトーシェント関数](https://ja.wikipedia.org/wiki/%E3%82%AA%E3%82%A4%E3%83%A9%E3%83%BC%E3%81%AE%CF%86%E9%96%A2%E6%95%B0) $\varphi(n)$| $p^e(1-p^{-1})$ |
|`Moebius<T>`| [メビウス関数](https://ja.wikipedia.org/wiki/%E3%83%A1%E3%83%93%E3%82%A6%E3%82%B9%E9%96%A2%E6%95%B0) $\mu(n)$|$\displaystyle \begin{cases}-1&e=1\newline 0& e\gt 1\end{cases}$|
|`Liouville<T>`| [Liouville 関数](https://en.wikipedia.org/wiki/Liouville_function) $\lambda(n)$ |$(-1)^e$|
|`Divisor<T,k>`|[約数関数](https://ja.wikipedia.org/wiki/%E7%B4%84%E6%95%B0%E9%96%A2%E6%95%B0) $\sigma_k(n)$ |$\displaystyle \sum_{i=0}^ep^{ki}$|
|`Dedekind<T>`| [Dedekind psi function](https://en.wikipedia.org/wiki/Dedekind_psi_function) $\psi(n)$|$p^e(1+p^{-1})$|

## 問題例
[Xmas Contest 2019 D - Sum of (-1)^f(n)](https://atcoder.jp/contests/xmascon19/tasks/xmascon19_d) ( $\lambda(n)$ ) \
[Xmas Contest 2019 E - Sum of f(n)](https://atcoder.jp/contests/xmascon19/tasks/xmascon19_e) ( $\Omega(n)$ ) 
## 参考
[https://en.wikipedia.org/wiki/Arithmetic_function](https://en.wikipedia.org/wiki/Arithmetic_function)\
[https://en.wikipedia.org/wiki/Additive_function](https://en.wikipedia.org/wiki/Additive_function)\
[https://en.wikipedia.org/wiki/Multiplicative_function](https://en.wikipedia.org/wiki/Multiplicative_function)