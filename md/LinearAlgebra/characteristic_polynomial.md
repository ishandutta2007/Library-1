---
title: 行列の特性多項式
documentation_of: ../../src/LinearAlgebra/characteristic_polynomial.hpp
---
行列の成分は 体 であること (特に乗法に関する逆元の存在) を前提にしている

| 関数                           | 概要                                                                                 | 計算量              |
| ------------------------------ | ------------------------------------------------------------------------------------ | ------------------- |
| `hessenberg(A)`                | 正方行列 $A$ ( `Matrix` クラス ) のヘッセンベルグ型を返す                            | $\mathcal{O} (n^3)$ |
| `characteristic_polynomial(A)` | 正方行列 $A$ ( `Matrix` クラス ) の特性多項式 $\phi(x)=\det(x \mathrm{I}-A)$ を返す. | $\mathcal{O} (n^3)$ |

