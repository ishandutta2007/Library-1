---
title: 直線
documentation_of: ../../src/Geometry/Line.hpp
---

## `Line<K>` クラス
二次元空間上の直線を表すクラス. \
`Point<K>` 型のpublicメンバ変数 `p`, `d` を持ち, 媒介変数 $t$ を用いた $\boldsymbol{p}+t\boldsymbol{d}$ で直線を表現している. \
点 $\boldsymbol{q}$ が 直線 $l: \boldsymbol{p}+t\boldsymbol{d}$ の左に位置するとは, ベクトル $\boldsymbol{q}-\boldsymbol{p}$ が $\boldsymbol{d}$ に対して反時計回りの時を指すこととする.

|メンバ関数|概要|
|---|---|
|`Line()`|デフォルトコンストラクタ|
|`Line(p,d)`|コンストラクタ. $\boldsymbol{p}+t\boldsymbol{d}$ で直線を定義.|
|`Line(a,b,c)`|コンストラクタ. $ax+by+c=0$ で直線を定義. <br> 点 $\boldsymbol{q}=(x,y)$ について $ax+by+c$ が 正, 負, 0 のとき, 点 $\boldsymbol{q}$ はそれぞれ 左, 右, 直線上 に位置することを意味する.|
|`where(q)`|点 $\boldsymbol{q}$ と直線の位置関係を表す. <br> +1: 直線の左, 0: 直線上, -1: 直線の右|
|`project(q)`|点 $\boldsymbol{q}$ から直線への垂線の足を返す.|
|`coef()`|$ax+by+c=0$と表した時の $a,b,c$ を返す.|

## その他関数

|名前|概要|
|---|---|
|`line_through(p,q)`|2点 $\boldsymbol{p},\boldsymbol{q}$ を通る直線を返す.|
|`is_parallel(l,m)`|2直線 $l,m$ が平行なら `true`.|
|`is_orthogonal(l,m)`|2直線 $l,m$ が垂直なら `true`.|
|`cross_points(l,m)`|2直線 $l,m$ の交点を返す. <br> 同一の直線なら直線上の2点を返し, 交わらないなら空集合を返す.<br> 返り値の型は `vector<Point<K>>`.|
|`bisector(p,q)`|点 $\boldsymbol{p},\boldsymbol{q}$ を端点とする線分の垂直二等分線を返す.|
|`bisector(l,m)`|直線 $l,m$ の角の二等分線を返す. <br> ただし平行ならちょうど真ん中を通る平行な直線を返す. <br> 通常は互いに垂直な2本を返す. <br> `Rational` は動かない. <br> 返り値の型は `vector<Line<K>>`. |
|`dist2(l,p)`<br>`dist2(p,l)`|直線 $l$ と 点 $\boldsymbol{p}$ の距離の二乗を返す.|
|`dist2(l,m)`|直線 $l$ と 直線 $m$ の距離の二乗を返す. ただし交わるときは 0.|
|`reflect(l)`|直線 $l$ を中心に折り返すようなアフィン変換を返す. 返り値の型は `Affine<K>`.|
