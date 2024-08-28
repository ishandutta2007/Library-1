---
title: ベクトル
documentation_of: ../../src/LinearAlgebra/Vector.hpp
---

## `Vector<K>` クラス

$K$ 上の $n$ 次元数ベクトル空間 $K^n$ のベクトルを表すクラス．

`K=bool` は特殊化して有限体 $\mathbb{F}_2$ の意味で用いる(定数倍高速化)


|  コンストラクタ | 概要 | 
| ---| --- |
|`Vector()`| 次元数 $0$ (invalid)|
|`Vector(n)`|　次元数 $n$ |
|`Vector(n,a)`| 次元数 $n$, 全成分が $a$ |
|`Vector({...})`| initializer_list|

|  メンバ関数 | 概要  | 計算量  |
| ---| --- | --- |
| `operator[](i)` | ランダムアクセス．第 $i$ 成分 $v_i$ を返す．| $O(1)$ |
| `size()` | ベクトルの次元数 $n$ を返す．| $O(1)$ |

|  演算子オーバーロード    | 概要  | 計算量  |
| ---| --- | --- |
| `operator-(v)` |$-\boldsymbol{v}$ を返す| $O(n)$ |
| `operator+(v,u)` |$\boldsymbol{v}+\boldsymbol{u}$ を返す| $O(n)$ |
| `operator-(v,u)` |$\boldsymbol{v}-\boldsymbol{u}$ を返す| $O(n)$ |
| `operator*(v,a)` |スカラー倍 $a\boldsymbol{v}$ を返す| $O(n)$ |
| `operator*(a,v)` |スカラー倍 $a\boldsymbol{v}$ を返す| $O(n)$ |
| `operator==(v,u)` |$\boldsymbol{v}=\boldsymbol{u}$ なら true| $O(n)$ |
| `operator!=(a,v)` |$\boldsymbol{v}\ne\boldsymbol{u}$ なら true| $O(n)$ |
| `operator+=(v,u)` |$\boldsymbol{v}\leftarrow\boldsymbol{v}+\boldsymbol{u}$| $O(n)$ |
| `operator-=(v,u)` |$\boldsymbol{v}\leftarrow\boldsymbol{v}-\boldsymbol{u}$| $O(n)$ |
| `operator*=(v,a)` |$\boldsymbol{v}\leftarrow a\boldsymbol{v}$| $O(n)$ |
| `bool(v)` |次元数が 1 以上なら true| $O(1)$ |


|  その他関数  | 概要  | 計算量  |
| ---| --- | --- |
|`dot(u,v)`|内積 $\boldsymbol{u}^{\top}\boldsymbol{v}$ を返す|$O(n)$|