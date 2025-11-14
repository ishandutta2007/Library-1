---
title: 行列の特性多項式 他
documentation_of: ../../src/LinearAlgebra/characteristic_polynomial.hpp
---



| 関数                           | 概要                                                                                 | 計算量              |
| ------------------------------ | ------------------------------------------------------------------------------------ | ------------------- |
| `hessenberg(A, mint=false)`                | 正方行列 $A$ ( `Matrix` クラス ) のヘッセンベルグ型を返す. <br> 第二引数 が `true`  : 互除法で計算. modintを前提にしてる. <br> 第二引数 が `false` : 乗法の逆元で計算. 体でないとダメ. デフォルト               | $O (n^3)$ |
| `characteristic_polynomial(A, mint=false)` | 正方行列 $A$ ( `Matrix` クラス ) の特性多項式 $\phi(x)=\det(x \mathrm{I}-A)$ を返す. <br> 第二引数 が `true`  : 互除法で計算. modintを前提にしてる. <br> 第二引数 が `false` : 乗法の逆元で計算. 体でないとダメ. デフォルト | $O (n^3)$ |
|`det_of_first_degree_poly_mat(M0, M1)` |$\det(M_0 + M_1x)$ を計算. <br>  体でないとダメ.|$O (n^3)$|


## 参考
[https://hitonanode.github.io/cplib-cpp/linear_algebra_matrix/hessenberg_reduction.hpp](https://hitonanode.github.io/cplib-cpp/linear_algebra_matrix/hessenberg_reduction.hpp)\
[https://hitonanode.github.io/cplib-cpp/linear_algebra_matrix/determinant_of_first_degree_poly_mat.hpp](https://hitonanode.github.io/cplib-cpp/linear_algebra_matrix/determinant_of_first_degree_poly_mat.hpp)

## Verify


- [Codeforces Round 459 (Div. 1) D. Stranger Trees](https://codeforces.com/contest/917/problem/D) (`det_of_first_degree_poly_mat`)

