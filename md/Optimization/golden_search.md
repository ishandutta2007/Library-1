---
title: 黄金分割探索
documentation_of: ../../src/Optimization/golden_search.hpp
---

引数の型が `long double` の単峰関数が対象. \
引数の型が整数な単峰関数を対象にする場合は[フィボナッチ探索](fibonacci_search.md) を使う.

## 関数

| 名前         | 概要                                                 | 計算量                         |
| ------------ | ---------------------------------------------------- | ------------------------------ |
| `golden_search<sgn>(f,l,r,iter=100)`   | $\lbrack l, r\rbrack$ 上で単峰となる関数 $f(x)$ の最適値とその最適解を返す. <br> 返り値は { 最適解 $x^\ast$, 最適値 $f(x^\ast)$ } <br> templateの引数で最大最小を指定. <br> 第四引数で反復回数を指定. デフォルトは100          | $n=r-l$ とおき, $f(x)$ の評価が$\mathcal{O}(A)$ かかるとしたとき<br>                  $\mathcal{O}(A\log n)$    |
