---
title: 線分
documentation_of: ../../src/Geometry/Segment.hpp
---

## `Segment<K>` クラス
二次元空間上の線分を表すクラス. \
`Point<K>` 型のpublicメンバ変数 `p`, `q` を持ち, それらを端点とした線分を表現している. 

|メンバ関数|概要|
|---|---|
|`Segment()`|デフォルトコンストラクタ|
|`Segment(p,q)`|コンストラクタ.|
|`operator[](i)`| `i==0` なら `p`, `i==1` なら `q` を返す.|
|`on(r)`| 点 $r$ が線分上にあるなら `true`.|
|`length()`|線分の長さを返す.|
|`closest_point(r)`| 点 $r$ から最も近い線分上の点を返す.|

## その他関数

|名前|概要|
|---|---|
|`cross_points(s,t)`|2線分 $s,t$ の交点を返す. <br> 重なるならその共通部分となる線分の端点(2点)を返す.<br> 交わらないなら空集合を返す.<br> 返り値の型は `vector<Point<K>>`.|
|`cross_points(s,l)` <br> `cross_points(l,s)`|線分 $s$ と直線 $l$ の交点を返す. <br> $s$ が $l$ 上にあるなら $s$の端点(2点)を返す.<br> 交わらないなら空集合を返す.<br> 返り値の型は `vector<Point<K>>`.|
|`intersection(s,t)`|2線分 $s,t$ の位置関係を4種類に分類. <br> `CROSSING`: 交差する. <br>`TOUCHING`: 一方の端点がもう一方に触れる. <br> `DISJOINT`: 交わらない. <br> `OVERLAP`: 長さが正の共通部分がある. |
|`dist2(s,p)` <br> `dist2(p,s)`| 線分 $s$ と点 $\boldsymbol{p}$ との距離の二乗|
|`dist2(s,l)` <br> `dist2(l,s)`| 線分 $s$ と直線 $l$ との距離の二乗. ただし交わるときは 0.|
|`dist2(s,t)`|線分 $s$ と線分 $t$ との距離の二乗. ただし交わるときは 0.|

