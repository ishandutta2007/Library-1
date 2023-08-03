---
title: 行列の特性多項式
documentation_of: ../../src/LinearAlgebra/characteristic_polynomial.hpp
---

第二引数 が `true`  : 互除法で計算. modintを前提にしてる. \
第二引数 が `false` : 乗法の逆元で計算. 体でないとダメ. デフォルト

| 関数                           | 概要                                                                                 | 計算量              |
| ------------------------------ | ------------------------------------------------------------------------------------ | ------------------- |
| `hessenberg(A, mint=false)`                | 正方行列 $A$ ( `Matrix` クラス ) のヘッセンベルグ型を返す                            | $\mathcal{O} (n^3)$ |
| `characteristic_polynomial(A, mint=false)` | 正方行列 $A$ ( `Matrix` クラス ) の特性多項式 $\phi(x)=\det(x \mathrm{I}-A)$ を返す. | $\mathcal{O} (n^3)$ |

