---
title: ディリクレ級数
documentation_of: ../../src/Math/DirichletSeries.hpp
---

$\newcommand{\floor}[1]{\left\lfloor{#1}\right\rfloor}$

ディリクレ級数の四則演算で作り上げた数論的関数の累積和を計算するためのもの.
## `DirichletSeries<T>` クラス
正の整数 $N$ が与えられるとする. <br>
ある数論的関数 $f$ と $k\in \lbrace a\in \mathbb{N}\; \vert\; \exist m \in \mathbb{N}, a=\floor{\frac{N}{m}}\rbrace$ について

$\displaystyle
F(k) = \sum_{n=1}^k f(n)
$

の値を格納する (と思うことにする) データ構造. \
$f$ や $F$ そのものは保持しない. \
またディリクレ級数として四則演算をサポート. \
以下 $K,L$ は 

$\displaystyle
K \simeq (N/\log N)^{2/3}, L\simeq N^{1/3}(\log N)^{2/3}
$

ぐらいの $N \leq KL$ を満たす正の整数を表す.

$O(K\log K+\sqrt{NL})=\mathcal{O}(N^{2/3}(\log N)^{1/3})$

またディリクレ級数の単元として次の表記を導入.

$\displaystyle 
\varepsilon(n) \equiv \lbrack n=1
\rbrack 
\equiv 
\begin{cases}
1& n=1, \newline
0& n \gt 1.
\end{cases}
$


|メンバ変数|概要|
|---|---|
|`x`| $f(1),\dots,f(K)$ の値を保存している配列. <br> サイズは$K+1$. 0番目の値は未定義.|
|`Xs`|$F(1),\dots,F(K)$ の値を保存している配列. <br> サイズは$K+1$. 0番目の値は未定義.|
|`Xl`|$F(\floor{N/1}),\dots,f(\floor{N/L})$ の値を保存している配列. <br> サイズは$L+1$. 0番目の値は未定義.|

|演算子オーバーロード|概要 ( $f,g$ から $h$ を返すイメージ )|計算量|
|---|---|---|
|`operator-(f)`|$h(n)=-f(n)$|$O(K+L)$|
|`operator+(f,g)`|$h(n)=f(n)+g(n)$|$O(K+L)$|
|`operator+(f,a)` |$h(n)=f(n)+a\cdot\varepsilon(n)$|$O(K+L)$|
|`operator+(a,f)` |$h(n)=a\cdot\varepsilon(n) + f(n)$　(上と同じ)|$O(K+L)$|
|`operator-(f,g)`|$h(n)=f(n)-g(n)$|$O(K+L)$|
|`operator-(f,a)`|$h(n)=f(n)-a\cdot\varepsilon(n)$|$O(K+L)$|
|`operator-(a,f)`|$h(n)=a\cdot\varepsilon(n)-f(n)$|$O(K+L)$|
|`operator*(f,g)`|$\displaystyle h(n)=\sum_{ij=n}f(i)g(j)$|$O(K\log K+\sqrt{NL})$|
|`operator*(f,a)`|$h(n)= f(n)\cdot a$|$O(K+L)$|
|`operator*(a,f)`|$h(n)=a\cdot f(n)$ (上と同じ)|$O(K+L)$|
|`operator/(f,g)`|$\displaystyle f(n) = \sum_{ij=n}h(i)g(j)$ を満たす $h$.|$O(K\log K+\sqrt{NL})$|
|`operator/(f,a)`|$h(n)=f(n) /a$|$O(K+L)$|
|`operator/(a,f)`|$\displaystyle a\cdot \varepsilon(n) = \sum_{ij=n}h(i)f(j)$ を満たす $h$.|$O(K\log K+\sqrt{NL})$|

|メンバ関数|概要|計算量|
|---|---|---|
|`DirichletSeries(N, x, Xs, Xl)`|コンストラクタ. $N$ と配列を直接渡して構築.||
|`DirichletSeries(N, unit=false)`|コンストラクタ. $N$ を渡して構築. <br> `unit` が `false` なら $f(n)\equiv0$, `true` なら $f(n)\equiv \varepsilon(n)$ のケースとして構築. <br> それぞれディリクレ級数の零元, 単元を意味する. ||
|`DirichletSeries(N, sum)`|コンストラクタ. $N$ と関数 $F$ を渡して構築. ||
|`sum(k)`| $F(k)$ を返す. <br>ただし <br> $\displaystyle k\in\left \lbrace a\in \mathbb{N}\; \vert\; \exist m \in \mathbb{N}, a=\floor{\frac{N}{m}}\right\rbrace$ <br>のケースのみ想定.|$\mathcal{O}(1)$|
|`sum()`|$\displaystyle F(N)=\sum_{n=1}^N f(n)$ を返す.|$\mathcal{O}(1)$
|`square()`|$\displaystyle h(n)=\sum_{ij=n}f(i)f(j)$ についての `DirichletSeries` クラスのオブジェクトを返す. <br> `operator*(f,f)` と同じ結果 (定数倍速い).|$O(K\log K+\sqrt{NL})$|
|`pow(M)`|$\displaystyle h(n)=\sum_{i_1\cdots i_M=n}f(i_1)\cdots f(i_M)$ についての `DirichletSeries` クラスのオブジェクトを返す. <br> つまりディリクレ積の $M$ 乗.|$\mathcal{O}(R(N)\log N)$<br> ただし $R(N)=\mathcal{O}(K\log K+\sqrt{NL})$|


## 具体的な数論的関数の　`DirichletSeries<T>`

$N$ を与えて, 具体的な数論的関数についての `DirichletSeries` クラスのオブジェクトを返す.

|関数|数論的関数|計算量|
|---|---|---|
|`get_1(N)`|$f(n)\equiv 1$| $\mathcal{O}(K+L)$|
|`get_mu(N)`|$f(n)\equiv\mu(n)$ <br> メビウス関数| $\mathcal{O}(K\log K+\sqrt{NL})$|
|`get_Id(N)`|$f(n)\equiv n$| $\mathcal{O}(K+L)$|
|`get_Id2(N)`|$f(n)\equiv n^2$| $\mathcal{O}(K+L)$|
|`get_d(N)`|$f(n)\equiv d(n)$ <br> 約数の個数関数| $\mathcal{O}(K\log K+\sqrt{NL})$|
|`get_sigma(N)`|$f(n)\equiv \sigma(n)$ <br> 約数の和関数| $\mathcal{O}(K\log K+\sqrt{NL})$|
|`get_phi(N)`|$f(n)\equiv \phi(n)$ <br> オイラーのトーシェント関数| $\mathcal{O}(K\log K+\sqrt{NL})$|
|`get_1sq(N)`|$f(n)\equiv \lbrack n \text{ is a square} \rbrack$ <br> $n$ が平方数の時のみ $1$, それ以外 $0$| $\mathcal{O}(K\log K+\sqrt{NL})$|
|`get_lambda(N)`|$f(n)\equiv\lambda(n)$ <br> Liouville のラムダ関数| $\mathcal{O}(K\log K+\sqrt{NL})$|
|`get_absmu(N)`|$f(n)\equiv\lvert\mu(n)\rvert$ <br> メビウス関数の絶対値.| $\mathcal{O}(K\log K+\sqrt{NL})$|


## 参考
[https://maspypy.com/dirichlet-積と、数論関数の累積和](https://maspypy.com/dirichlet-%E7%A9%8D%E3%81%A8%E3%80%81%E6%95%B0%E8%AB%96%E9%96%A2%E6%95%B0%E3%81%AE%E7%B4%AF%E7%A9%8D%E5%92%8C)
## 問題例
[Xmas Contest 2019 D - Sum of (-1)^f(n)](https://atcoder.jp/contests/xmascon19/tasks/xmascon19_d) (Liouville)\
[AtCoder Beginner Contest 239 Ex - Dice Product 2](https://atcoder.jp/contests/abc239/tasks/abc239_h) \
[LibreOJ #6491. 「XXOI 2018」简单的最大公约数](https://loj.ac/p/6491) \
[LibreOJ #6686. Stupid GCD](https://loj.ac/p/6686) \
[LibreOJ #6714. Stupid Product](https://loj.ac/p/6714)\
[LibreOJ #572. 「LibreOJ Round #11」Misaka Network 与求和](https://loj.ac/p/572) (prime count もいる)