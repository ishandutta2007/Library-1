---
title: 凸多角形
documentation_of: ../../src/Geometry/Convex.hpp
---

## `Convex<K>` クラス
二次元空間上の凸多角形を表すクラス. \
頂点が反時計回りになるようにしてある. \
頂点数を $n$ とする.

|メンバ関数|概要|計算量|
|---|---|---|
|`Convex()`|デフォルトコンストラクタ||
|`Convex(ps,strict)`|コンストラクタ. <br> 引数として `vector<Point<K>>` で点群を与え, 凸包を実行することで凸多角形を作る. <br> 第二引数が`false`なら内角として180度を認める.デフォルトは`true`.|$\mathcal{O}(m\log m)$ <br> ただし引数の点の個数を $m$ とした|
|`farthest_pair()`| 頂点のペアのうち2点間の距離が最も遠いものを返す. |$\mathcal{O}(n)$|
|`diameter()`|凸多角形の直径(凸多角形の内部に収まる線分の長さの最大値)を返す. <br> `Rational` は動かない.|$\mathcal{O}(n)$|
|`cut(l, side)`|直線 $l$ で分けられる半平面のうちの一方との共通部分を返す(これもまた凸多角形). <br> 第二引数が正なら直線の左側, 負なら右側, デフォルトは正. |$\mathcal{O}(n)$|
|`cut(a, b, c, side)`| 半平面 $(ax+by+c)\cdot\mathrm{side} \geq 0$ との共通部分を返す(これもまた凸多角形). <br> `side` のデフォルトは正. |$\mathcal{O}(n)$|

## その他関数

|名前|概要|計算量|
|---|---|---|
|`farthest_pair(ps)`| 引数は `vector<Point<K>>`. <br>点群のペアのうち2点間の距離が最も遠いもの (最遠点対) を返す. |$\mathcal{O}(n \log n)$ <br> ただし引数の点の個数を $n$ とした|

