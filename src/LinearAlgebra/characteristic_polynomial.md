---
title: 行列の特性多項式
documentation_of: ./characteristic_polynomial.hpp
---

| 関数                           | 概要                                                                                 | 計算量              |
| ------------------------------ | ------------------------------------------------------------------------------------ | ------------------- |
| `hessenberg(A)`                | 正方行列 $A$ ( `Matrix` クラス ) のヘッセンベルグ型を返す                            | $\mathcal{O} (n^3)$ |
| `characteristic_polynomial(A)` | 正方行列 $A$ ( `Matrix` クラス ) の特性多項式 $\phi(x)=\det(x \mathrm{I}-A)$ を返す. | $\mathcal{O} (n^3)$ |

