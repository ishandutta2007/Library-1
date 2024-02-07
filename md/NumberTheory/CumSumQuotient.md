---
title: 約数配列
documentation_of: ../../src/NumberTheory/CumSumQuotient.hpp
---

$\newcommand{\floor}[1]{\left\lfloor{#1}\right\rfloor}$


## `CumSumQuotient<T>` クラス

添字が

$
\displaystyle
S =\left\lbrace \left. \floor{\frac{N}{x}} \right \vert x\in \mathbb{Z}, 1\leq x\leq N\right\rbrace
$

の要素のみの配列.

特に数論的関数 $f(\cdot)$ の $n\in S$ までの累積和

$
\displaystyle
F(n) = \sum_{i=1}^nf(i)
$

の値を格納するのに用いる.

ベクトル的な演算 (加減算，スカラー倍) に対応.


|メンバ変数|概要|
|---|---|
|`N`|$N$|
|`K`|$K=\floor{\sqrt{N}}$|
|`X`|実体. `valarray`. 直接触ることはない|

||概要 |計算量|
|---|---|---|
|`operator-(F)`|$-F(n)$|$\Omicron(\sqrt{N})$|
|`operator+(F,G)`|$F(n)+G(n)$|$\Omicron(\sqrt{N})$|
|`operator-(F,G)`|$F(n)-G(n)$|$\Omicron(\sqrt{N})$|
|`operator*(F,a)`|$F(n)\cdot a$|$\Omicron(\sqrt{N})$|
|`operator*(a,F)`|$a\cdot F(n)$ (上と同じ)|$\Omicron(\sqrt{N})$|


|メンバ関数|概要|
|---|---|
|`CumSumQuotient(N)`|コンストラクタ. $N$ を渡して構築. 要素はデフォルト値. |
|`operator[](n)`| $F(n)$ の値を左参照で返す. <br> $n\in \left\lbrace \left. \floor{\frac{N}{x}} \right \vert x\in \mathbb{Z}, 1\leq x\leq N\right\rbrace$ を想定.|
|`operator()(n)`| $F(n)$ の値を返す. <br> $n\in \left\lbrace \left. \floor{\frac{N}{x}} \right \vert x\in \mathbb{Z}, 1\leq x\leq N\right\rbrace$ を想定.|
|`sum()`|$\displaystyle F(N)= \sum_{i=1}^N f(i)$ を返す.|
|`add(i, v)`| $f(i)\leftarrow f(i)+v$|


