---
title: 行列
documentation_of: ../../src/LinearAlgebra/Matrix.hpp
---

## `Matrix<R>` クラス

半環 $R$ 上の行列を表すクラス. \
`R=bool` は特殊化して有限体 $\mathbb{F}_2$ の意味で用いる. (定数倍高速化) \
`Vector` との乗算をサポート. 線型写像と思って `operotor()` でも記述可.\
`Matrix` 同士の加算・乗算をサポート. 

| コンストラクタ | 概要  |
| ---| --- | 
|`Matrix()`|$0\times 0$ 行列 (invalid)|
|`Matrix(n,m)`|$n\times m$ 行列|
|`Matrix(n,m,a)`|$n\times m$ 行列, 全成分が $a$|
|`Matrix(initializer_list<initializer_list<R>>)`|initializer_list|


| static メンバ関数 | 概要  |
| ---| --- |
|`identity(n)`| $n\times n$ 単位行列を返す |

|  メンバ関数 | 概要  | 計算量  |
| ---| --- | --- |
| `operator[](i)` | ランダムアクセス．第 $i$ 行を返す．| $O(1)$ |
| `height()` | 行数 $n$ を返す．| $O(1)$ |
| `width()` | 列数 $m$ を返す．| $O(1)$ |
| `pow(k)` |  $A^k$ を返す．| $O(n^3\log k)$ |
| `submatrix(rows,cols)` |行列 $A$ の小行列を返す． 行と列を指定して，それが残るようにする． |  |
| `submatrix_rm(rows,cols)` |行列 $A$ の小行列を返す． 行と列を指定して，それを除くようにする． |  |

|  演算子オーバーロード    | 概要  | 計算量  |
| ---| --- | --- |
| `operator+(A,B)` |$A+B$ を返す| $O(nm)$ |
| `operator*(A,a)` |スカラー倍 $aA$ を返す| $O(nm)$ |
| `operator*(a,A)` |スカラー倍 $aA$ を返す| $O(nm)$ |
| `operator*(A,B)` |$AB$ を返す| $O(nml)$ <br> ただし $B\in R^{m\times l}$ とした |
| `operator*(A,v)` |$A\boldsymbol{v}$ を返す. 返り値は `Vector<R>`| $O(nm)$ |
| `operator==(A,B)` |$A=B$ なら true| $O(nm)$ |
| `operator!=(A,B)` |$A\ne B$ なら true| $O(nm)$ |
| `operator+=(A,B)` |$A\leftarrow A+B$| $O(nm)$ |
| `operator-=(A,B)` |$A\leftarrow A-B$| $O(nm)$ |
| `operator*=(A,a)` |$A\leftarrow aA$| $O(nm)$ |
| `bool(A)` |列数が 1 以上なら true| $O(1)$ |


## Verify
<details>
<summary>ここをクリックすると開きます</summary>

- [ ] [AtCoder Beginner Contest 009 D - 漸化式](https://atcoder.jp/contests/abc009/tasks/abc009_4) (bitwise xor and 半環)
- [ ] [AtCoder Regular Contest 027 D - ぴょんぴょんトレーニング](https://atcoder.jp/contests/arc027/tasks/arc027_4) (セグ木(MLギリギリ))
- [ ] [「みんなのプロコン 2019」決勝 D - Dangerous Hopscotch](https://atcoder.jp/contests/yahoo-procon2019-final/tasks/yahoo_procon2019_final_d)
- [ ] [DISCO presents ディスカバリーチャンネル コードコンテスト2019 本戦 D - DISCO!](https://atcoder.jp/contests/ddcc2019-final/tasks/ddcc2019_final_d)
- [ ] [DISCO presents ディスカバリーチャンネル コードコンテスト2020 本戦 B - Hawker on Graph](https://atcoder.jp/contests/ddcc2020-final/tasks/ddcc2020_final_b) (max+の一次関数の加法合成の半環)

</details>