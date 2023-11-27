---
title: 拡張エラトステネス篩
documentation_of: ../../src/NumberTheory/ExtendedEratosthenesSieve.hpp
---

$\newcommand{\floor}[1]{\left\lfloor{#1}\right\rfloor}$


## `PrimeSum<T>` クラス
正の整数 $N$ が与えられるとする. <br>
ある数論的関数 $f$ と $k\in \lbrace a\in \mathbb{N}\; \vert\; \exists m \in \mathbb{N}, a=\floor{\frac{N}{m}}\rbrace$ について

$\displaystyle
F_p(k) = \sum_{\substack{p\leq k\newline p \text{ is a prime}}} f(p)
$

の値を格納する (と思うことにする) データ構造. \
$f$ や $F$ そのものは保持しない. \
加減算とスカラー倍の演算子をサポート.

|メンバ変数|概要|
|---|---|
|`N`|$N$|
|`K`|$K=\floor{\sqrt{N}}$|
|`Xs`|$F_p(1),\dots,F_p(K)$ の値を保存している配列. <br> サイズは$K+1$. 0番目の値は未定義.|
|`Xl`|$F_p(\floor{N/1}),\dots,F_p(\floor{N/L})$ の値を保存している配列. <br> サイズは$L+1$. 0番目の値は未定義.|

|演算子オーバーロード|概要 ( $f,g$ から $h$ を返すイメージ )|計算量|
|---|---|---|
|`operator-(f)`|$h(n)=-f(n)$|$O(K+L)$|
|`operator+(f,g)`|$h(n)=f(n)+g(n)$|$O(K+L)$|
|`operator-(f,g)`|$h(n)=f(n)-g(n)$|$O(K+L)$|
|`operator*(f,a)`|$h(n)= f(n)\cdot a$|$O(K+L)$|
|`operator*(a,f)`|$h(n)=a\cdot f(n)$ (上と同じ)|$O(K+L)$|


|メンバ関数|概要|
|---|---|
|`PrimeSum(N, Xs, Xl)`|コンストラクタ. $N$ と配列を直接渡して構築.||
|`PrimeSum(N=0)`|コンストラクタ. $N$ を渡して構築. <br> $f(n)\equiv \varepsilon(n)$ のケースとして構築. |
|`sum(k)`| $F_p(k)$ を返す. <br>ただし <br> $\displaystyle k\in\left \lbrace a\in \mathbb{N}\; \vert\; \exists m \in \mathbb{N}, a=\floor{\frac{N}{m}}\right\rbrace$ <br>のケースのみ想定.|
|`sum()`|$\displaystyle F_p(N)= \sum_{\substack{p\leq N\newline p \text{ is a prime}}} f(p)$ を返す.|
|`add(p, v)`| $f(p)\leftarrow f(p)+v$ <br> $p$ は素数を想定. |

## `ExtendedEratosthenesSieve<T>` クラス

コンストラクタで前処理をしておいて, 乗法的関数や加法的関数の累積和を求める関数を提供するクラス.

|メンバ変数|概要|
|---|---|
|`primes`|$\sqrt{N}$ 以下の素数が列挙されている `vector`. |
|`pwsum`|`pwsum[d]` が $f(p)\equiv p^d$ についての `PrimeSum` クラスのオブジェクト. <br> `vector<PrimeSum>` 型. |

|メンバ関数|概要|計算量|
|---|---|---|
|`ExtendedEratosthenesSieve(N,D)`|$N$ と `pwsum` の最大次元 $D$ を渡して構築. <br> 線形篩を計算する. |$\displaystyle \mathcal{O}\left(\frac{N^{3/4}}{\log N}\right)$|
|`addtive_sum(f, X)`|加法的関数 $f$ について累積和 $\displaystyle \sum_{n=1}^N f(n)$ を返す. <br> $f(p^e)$ を表す `f(p,e)` とその $f$ に関する `PrimeSum` クラスのオブジェクト `X` を渡す. | $\mathcal{O}(\sqrt{N})$ |
|`addtive_sum(f, poly)`|加法的関数 $f$ について累積和 $\displaystyle \sum_{n=1}^N f(n)$ を返す. <br> $f(p^e)$ を表す `f(p,e)` と $f(p)$ の $p$ の多項式の係数列 `poly` を渡す. | $\mathcal{O}(\sqrt{N})$ |
|`multiplicative_sum(f, X)`|乗法的関数 $f$ について累積和 $\displaystyle \sum_{n=1}^N f(n)$ を返す. <br> $f(p^e)$ を表す `f(p,e)` とその $f$ に関する `PrimeSum` クラスのオブジェクト `X` を渡す. | $\displaystyle \mathcal{O}\left(\frac{N^{3/4}}{\log N}\right)$ |
|`multiplicative_sum(f, poly)`|乗法的関数 $f$ について累積和 $\displaystyle \sum_{n=1}^N f(n)$ を返す. <br> $f(p^e)$ を表す `f(p,e)` と $f(p)$ の $p$ の多項式の係数列 `poly` を渡す. | $\displaystyle \mathcal{O}\left(\frac{N^{3/4}}{\log N}\right)$ |

## 問題例
[Xmas Contest 2019 D - Sum of (-1)^f(n)](https://atcoder.jp/contests/xmascon19/tasks/xmascon19_d) \
[Xmas Contest 2019 E - Sum of f(n)](https://atcoder.jp/contests/xmascon19/tasks/xmascon19_e) (加法的関数) \
[LibreOJ #572. 「LibreOJ Round #11」Misaka Network 与求和](https://loj.ac/p/572) \
[LibreOJ #6686. Stupid GCD](https://loj.ac/p/6686) (2次多項式) \
[LibreOJ #6053. 简单的函数](https://loj.ac/p/6053) (単純な多項式でない)
