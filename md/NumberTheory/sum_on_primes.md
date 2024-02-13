---
title: 素数上の累積和
documentation_of: ../../src/NumberTheory/sum_on_primes.hpp
---

$\newcommand{\floor}[1]{\left\lfloor{#1}\right\rfloor}$

加法的関数もしくは乗法的関数 $f$ について，その累積和

$
\displaystyle
F(N) = \sum_{i=1}^N f(i)
$

を計算するための関数群．

数論的関数 $f$ について，素数上で $n\in \left\lbrace \left. \floor{\frac{N}{x}} \right \vert x\in \mathbb{Z}, 1\leq x\leq N\right\rbrace$ まで累積和をとったもの

$
\displaystyle
P(n) = \sum_{\substack{p: \mathrm{prime} \\ p \leq n}} f(p)
$

を利用することで $F(n)$ を計算する. \
$P(n)$ は[`CumSumQuotient<T>` クラス](CumSumQuotient.hpp) として扱う.


|関数名|概要|計算量|
|---|---|---|
|`sums_of_powers_on_primes<T>(N,D)`| $n\in \left\lbrace \left. \floor{\frac{N}{x}} \right \vert x\in \mathbb{Z}, 1\leq x\leq N\right\rbrace$ までの <br> 素数上の $k$ 乗数の累積和 <br> $\displaystyle \sum_{\substack{p: \mathrm{prime} \\ p \leq n}} p^k$ <br>を $k=0,\dots,D$ まで計算したものを返す. <br> 特に $k=0$ は素数計数関数 $\pi(n)$ となる. <br> 返り値は`vector<CumSumQuotient<T>>`.|$\displaystyle O\left(\frac{DN^{3/4}}{\log N}\right)$|
|`additive_sum(P,f)`|加法的関数 $f$ について累積和 $F(N)$ を返す. <br> $\displaystyle P(n)=\sum_{\substack{p: \mathrm{prime} \\ p \leq n}} f(p)$ を表す `CumSumQuotient<T>` クラスの `P` と $f(p^e)$ を表す `f(p,e)`を渡す．|$\displaystyle O\left(\sqrt{N}\right)$|
|`multiplicative_sum(P,f)`|乗法的関数 $f$ について累積和 $F(N)$ を返す. <br> $\displaystyle P(n)=\sum_{\substack{p: \mathrm{prime} \\ p \leq n}} f(p)$ を表す `CumSumQuotient<T>` クラスの `P` と $f(p^e)$ を表す `f(p,e)`を渡す．|$\displaystyle O\left(\frac{DN^{3/4}}{\log N}\right)$|

## 問題例
[Xmas Contest 2019 D - Sum of (-1)^f(n)](https://atcoder.jp/contests/xmascon19/tasks/xmascon19_d) \
[Xmas Contest 2019 E - Sum of f(n)](https://atcoder.jp/contests/xmascon19/tasks/xmascon19_e) (加法的関数) \
[LibreOJ #572. 「LibreOJ Round #11」Misaka Network 与求和](https://loj.ac/p/572) \
[LibreOJ #6686. Stupid GCD](https://loj.ac/p/6686) ($f(p)$ が2次多項式) \
[LibreOJ #6053. 简单的函数](https://loj.ac/p/6053) ($f(p)$ が単純な多項式でない)

## 参考
[https://oi-wiki.org/math/number-theory/min-25/](https://oi-wiki.org/math/number-theory/min-25/)