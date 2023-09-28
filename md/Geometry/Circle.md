---
title: 円
documentation_of: ../../src/Geometry/Circle.hpp
---

## `Circle<R>` クラス
二次元空間上の円を表すクラス. \
中心を表す `Point<R>` 型のpublicメンバ変数 `o` と半径を表す `R` 型のpublicメンバ変数 `r` を持つ. \
スカラーは浮動小数点数が望ましい.

|メンバ関数|概要|
|---|---|
|`Circle()`|デフォルトコンストラクタ|
|`Circle(o,r)`|コンストラクタ.|
|`area()`| 面積を返す. |
|`where(p)`| 点 $\boldsymbol{p}$ と円の位置関係を表す. <br> +1: 円の内部, 0: 円周上, -1: 円の外部|
|`where(l)`| 直線 $l$ と円の位置関係を表す. <br> +1: 2点で交わる, 0: 接する, -1: 交わらない|
|`in(c)`|円 $c$ がこの円の中に収まるなら `true`|
|`tangent(p)`| 点 $p$ を通る円の接線を全て返す. <br> `Rational` は動かない. <br> 返り値の型は `vector<Line<R>>`|

## その他関数
どれも `Rational` は動かない.

|名前|概要|
|---|---|
|`cross_points(c,d)`|2円 $c,d$ の交点を返す. <br> 通常は交点を2点返すが( $\boldsymbol{p}, \boldsymbol{q}$ の順とする), 円 $c$ の周上を $\boldsymbol{p}\rightarrow\boldsymbol{q}$ と反時計回りに結んだ弧が円 $d$ の内部にあるようにしてある. <br> 円が一致するなら円周上の3点を返す. <br> 接するなら接点1点を返す. <br> 交わらないなら空集合を返す.<br> 返り値の型は `vector<Point<R>>`.|
|`cross_points(c,l)` <br> `cross_points(l,c)`|円 $c$ と直線 $l$ の交点を全て返す. <br> 返り値の型は `vector<Point<R>>`.|
|`cross_points(c,s)` <br> `cross_points(s,c)`|円 $c$ と線分 $s$ の交点を全て返す. <br> 返り値の型は `vector<Point<R>>`.|
|`circumscribed_circle(A,B,C)`| 三角形 $ABC$ の外接円を返す.|
|`inscribed_circle(A,B,C)`| 三角形 $ABC$ の内接円を返す.|
|`common_tangent(c,d)`| 2円 $c,d$ の共通接線を全て返す. <br> 返り値の型は `vector<Line<R>>`|