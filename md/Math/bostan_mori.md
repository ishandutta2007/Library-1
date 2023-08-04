---
title: 線形漸化的数列の第$k$項
documentation_of: ../../src/Math/bostan_mori.hpp
---

|名前|概要|計算量|
|---|---|---|
|`div_at(p,q,k)`| \\[\lbrack x^k\rbrack\frac{p(x)}{q(x)}\\] | $\mathcal{O}(d^2\log k)$ <br> ただし $d = \max(\deg(p), \deg(q))$|
|`linear_recurrence(c,a,k)`| 線形漸化式 \\[b_n = \begin{cases}a_n & n<d\\c_0 b_{n-1} + c_1 b_{n-2} + ... + c_{d-1} b_{n-d} & n \geq d \end{cases}\\] を満たす数列 $(b_n)$ の第 $k$ 項 $b_k$ の値を返す. |$\mathcal{O}(d^2\log k)$|

## 参考
[線形漸化的数列のN項目の計算](https://qiita.com/ryuhe1/items/da5acbcce4ac1911f47a)