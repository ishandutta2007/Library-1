---
title: フィボナッチ探索
documentation_of: ../../src/Optimization/fibonacci_search.hpp
---
## 関数

| 名前         | 概要                                                 | 計算量                         |
| ------------ | ---------------------------------------------------- | ------------------------------ |
| `fibonacci_search<sgn>(f,l,r)`   | $\lbrack l, r\rbrack \cap\mathbb{Z}$ 上で単峰となる関数 $f(x)$ の最適値とその最適解を返す. <br> 返り値は { 最適解 $x^*$, 最適値 $f(x^*)$ } <br> templateの引数で最大最小を指定.           | $n=r-l$ とおき, $f(x)$ の評価が$\mathcal{O}(A)$ かかるとしたとき<br>                  $\mathcal{O}(A\log n)$    |

## 参考
[https://qiita.com/tanaka-a/items/f380257328da421c6584](https://qiita.com/tanaka-a/items/f380257328da421c6584) 