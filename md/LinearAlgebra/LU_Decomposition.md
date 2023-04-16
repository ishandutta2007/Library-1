---
title: LU分解
documentation_of: ../../src/LinearAlgebra/LU_Decomposition.hpp
---
`bool` は特殊化して有限体 $\mathbb{F}_2$ の意味で用いる \
その他の場合も行列の成分は 体 であること (特に乗法に関する逆元の存在) を前提にしている


## メンバ関数

| 関数名                | 概要                                                                                                                                                        | 計算量             |
| --------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------ |
| `LU_Decomposition(A)` | コンストラクタ. 行列 $A$ ( `Matrix` クラス ) のLU分解を実行.                                                                                                | $\mathcal{O}(n^3)$ |
| `rank()`              | 行列のランク $\mathrm{rank}\; A$ を返す                                                                                                                     | $\mathcal{O}(1)$   |
| `is_regular()`        | 行列が正則なら true                                                                                                                                         | $\mathcal{O}(1)$   |
| `det()`               | 行列式 $\det A$ の値を返す                                                                                                                                  | $\mathcal{O}(n)$   |
| `kernel()`            | 行列のカーネル $\mathrm{Ker}\;A \stackrel{\mathrm{def}}{=} \lbrace \boldsymbol{x}: A\boldsymbol{x}=\boldsymbol{0}\rbrace$ を返す. ( `Vector` クラスの集合 ) | $\mathcal{O}(n^3)$ |
| `linear_equations(b)` | 線形方程式 $A\boldsymbol{x}=\boldsymbol{b}$ の解の一つ ( `Vector` クラス ) を返す. 存在しないならサイズ0のものを返す                                        | $\mathcal{O}(n^2)$ |
| `inverse_matrix()`    | 逆行列 $A^{-1}$ を返す. 存在しないなら高さ・幅 0の `Matrix` を返す                                                                                          | $\mathcal{O}(n^3)$ |
