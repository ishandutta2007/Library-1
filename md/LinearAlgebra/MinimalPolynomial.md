---
title: 行列の最小多項式
documentation_of: ../../src/LinearAlgebra/MinimalPolynomial.hpp
---

## `MinimalPolynomial` クラス
正方行列 $M$ と ベクトル $\boldsymbol{b}$ を与えて 最小多項式 $p(x)=p_0+p_1x+\cdots+p_{d-1}x^{d-1}+x^d$ を求める. \
ただしここでの最小多項式とは、モニックな多項式であって
\\[
p(M)\boldsymbol{b} = p_0\boldsymbol{b}+p_1M\boldsymbol{b}+\cdots+p_{d-1}M^{d-1}\boldsymbol{b} + M^d\boldsymbol{b} = \boldsymbol{0}
\\]
を満たす最小次数のものを指す.

`ModInt` クラスを前提にしている.

### メンバ関数

| 関数                     | 概要                                                                                                                                                 | 計算量                                                                         |
| ------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------ |
| `MinimalPolynomial(M,b)` | コンストラクタ. 表現行列が $n\times n$ 正方行列な線型写像 $M$ ( `Vector` $\rightarrow$ `Vector` の関数 or `Matrix` クラス) とベクトル $\boldsymbol{b}$ ( `Vector` クラス ) を与えて最小多項式を求める | $O(n^2+nT(n))$<br> ただし $T(n)$ は $M$ とベクトルの乗算にかかる時間 |
| `degree()`               | 最小多項式の次元を返す                                                                                                                               | $O(1)$                                                               |
| `operator[](i)`          | 最小多項式の$x^i$の係数を返す                                                                                                                        | $O(1)$                                                               |
| `pow(k)`                 | $M^k\boldsymbol{b}$ ( `Vector` クラス ) を返す                                                                                                       | $O(n^2\log k)$                                                       |

## その他関数

| 関数     | 概要                                                                                   | 計算量                                                                         |
| -------- | -------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------ |
| `linear_map_det<mod_t>(M,n)` | 表現行列が $n\times n$ 正方行列な線型写像 $M$ ( `Vector` $\rightarrow$ `Vector` の関数 or `Matrix` クラス) の行列式 $\det M$ の値を返す | $O(n^2+nT(n))$<br> ただし $T(n)$ は $M$ とベクトルの乗算にかかる時間 |


## 参考
[https://yukicoder.me/wiki/black_box_linear_algebra](https://yukicoder.me/wiki/black_box_linear_algebra)