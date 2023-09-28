---
title: 点
documentation_of: ../../src/Geometry/Point.hpp
---

二次元幾何のベースとなる部分. \
一応三角関数や平方根を使わないものに関しては `Rational` クラスも乗せられるはず. \
名前空間 `geo` 内で全て定義している.
## 関数

|名前|概要|
|---|---|
|`sgn(x)`|誤差を考慮するための関数. <br> 負なら-1, 正なら+1, それ以外なら0を返す. <br> ただし浮動小数点数の場合絶対値が $10^{-9}$ 以下なら0とみなす. |
|`err_floor(x)`|床関数. `sgn` に従って結果を返す.|
|`err_ceil(x)`|天井関数. `sgn` に従って結果を返す.|

## `Point<K>` クラス
二次元空間上の点およびベクトルを表すクラス. \
publicメンバ変数 `x`, `y` を持つ. \
辞書順によって不等号を定義している. (`set` とか `map` とかの keyにできる.) \
`!` 演算子で原点を中心に $\pi/2$ だけ回転したものを返すようにした

## `Point<K>` クラスを引数に取る関数

|名前|概要|
|---|---|
|`dot(p,q)`| $\boldsymbol{p}\cdot \boldsymbol{q}$ |
|`cross(p,q)`|$\boldsymbol{p}\times \boldsymbol{q}$ <br> $\boldsymbol{p}$ から $\boldsymbol{q}$ へ反時計回りなら正.|
|`norm2(p)`|$\lVert \boldsymbol{p}\rVert^2$|
|`norm(p)`|$\lVert \boldsymbol{p}\rVert$ <br> `Rational` は動かない.|
|`dist2(p,q)`|点 $\boldsymbol{p}$, $\boldsymbol{q}$ 間のユークリッド距離の二乗.|
|`dist(a,b)`|オブジェクト $a$, $b$ 間のユークリッド距離. <br> `Rational` は動かない. <br> `dist2` の結果を `sqrt` したもの. <br> 直線・線分・多角形なども `dist2` を定義することで自動的にこれも定義される.|
|`ccw(p0,p1,p2)`|$\boldsymbol{p}_0\rightarrow \boldsymbol{p}_1\rightarrow \boldsymbol{p}_2$ の順に訪れた時の軌道を5種類に分類. <br> `COUNTER_CLOCKWISE`: 左折 <br> `CLOCKWISE`: 右折 <br> `ONLINE_FRONT`: 直進 <br> `ONLINE_BACK`: Uターン ( $\boldsymbol{p}_2$ が $\boldsymbol{p}_0$ より後ろ )　<br>  `ON_SEGMENT`: Uターン( $\boldsymbol{p}_2$ が $\boldsymbol{p}_0$ と $\boldsymbol{p}_1$ の間)  |

## `Affine<K>` クラス
アフィン変換のための関数オブジェクト.\
が、使いそうな変換は平行移動・回転・折り返しを合成したものくらいか\
`operator()(x)` で点,直線,線分,多角形を変換する. \
`operator*(f,g)` で合成 $f\circ g = f(g(x))$

## `Affine<K>` を返す関数

|名前|概要|
|---|---|
|`translate(p)`| ベクトル $\boldsymbol{p}$ だけ平行移動する変換を返す.|