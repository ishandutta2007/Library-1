---
title: LU分解
documentation_of: ../../src/LinearAlgebra/LU_Decomposition.hpp
---


## `LU_Decomposition<K>` クラス

`K=bool` は特殊化して有限体 $\mathbb{F}_2$ の意味で用いる(定数倍高速化) \
その他の場合も行列の成分 `K` は 体 であることを前提にしている

| メンバ関数             | 概要                                                                                                                                                        | 計算量             |
| --------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------ |
| `LU_Decomposition(A)` | コンストラクタ. 行列 $A$ ( `Matrix` クラス ) のLU分解を実行.                                                                                                | $O(n^3)$ |
| `rank()`              | 行列のランク $\mathrm{rank}\; A$ を返す                                                                                                                     | $O(1)$   |
| `is_regular()`        | 行列が正則なら true                                                                                                                                         | $O(1)$   |
| `det()`               | 行列式 $\det A$ の値を返す                                                                                                                                  | $O(n)$   |
| `kernel()`            | 行列のカーネル $\mathrm{Ker}\;A \stackrel{\mathrm{def}}{=} \lbrace \boldsymbol{x}: A\boldsymbol{x}=\boldsymbol{0}\rbrace$ を返す. ( `Vector` クラスの集合 ) | $O(n^3)$ |
| `linear_equations(b)` | 線形方程式 $A\boldsymbol{x}=\boldsymbol{b}$ の解の一つ ( `Vector` クラス ) を返す. 存在しないならサイズ0のものを返す                                        | $O(n^2)$ |
| `inverse_matrix()`    | 逆行列 $A^{-1}$ を返す. 存在しないなら高さ・幅 0の `Matrix` を返す                                                                                          | $O(n^3)$ |

## 問題例
[yukicoder No.1421 国勢調査 (Hard)](https://yukicoder.me/problems/no/1421) (sp judge)