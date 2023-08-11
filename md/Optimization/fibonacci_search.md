---
title: フィボナッチ探索
documentation_of: ../../src/Optimization/fibonacci_search.hpp
---
templateの第二引数で最大最小を指定. 
## 関数

| 名前         | 概要                                                 | 計算量                         |
| ------------ | ---------------------------------------------------- | ------------------------------ |
| `fibonacci_search<T, sgn>(f,l,r)`   | 凸関数 $f(x)$ の $x\in\lbrack l, r\rbrack$ における最大値(最小値) を返す.            |      $\mathcal{O}(A\log n)$  <br> ただし $n=r-l$, $f(x)$ の評価が$\mathcal{O}(A)$                       |

## 参考
[https://qiita.com/tanaka-a/items/f380257328da421c6584](https://qiita.com/tanaka-a/items/f380257328da421c6584) 