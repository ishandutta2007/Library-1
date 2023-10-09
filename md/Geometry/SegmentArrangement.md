---
title: 線分アレンジメント
documentation_of: ../../src/Geometry/SegmentArrangement.hpp
---

## `SegmentArrangement<K>` クラス
二次元空間上の線分の端点および交点を頂点とし, 線分上をたどって辿り着ける点どうしが連結するように辺を張ってグラフを構築する線分アレンジメントを doubly connected edge list (DCEL) で表現するクラス. 

|メンバ関数|概要|
|---|---|
|`SegmentArrangement(ss)`|コンストラクタ. `vector<Segment<K>>` を引数にとる. 線分アレンジメントを実行. | 
|`vertex_size()`|頂点数を返す.|
|`face_size()`|面数を返す.|
|`out_edges(v)`|頂点 v から出る辺を隣接リストのように返す.|
|`point(v)`|頂点 v に対応する二次元空間上の点 ( `Point<K>` クラス ) を返す.|
|`vertex(p)`|二次元空間上の点 ( `Point<K>` クラス ) $\boldsymbol{p}$ に対応する頂点番号を返す. |
|`to_v(e)`| 有向辺 e の行き先の頂点を返す.|
|`from_v(e)`|有向辺 e が出てくる頂点を返す.|
|`twin(e)`|有向辺 e の頂点を逆にした有向辺を返す.|
|`next(e)`|有向辺 e の次の有向辺を返す. この順というのは反時計回りに多角形面の辺を辿る順を指す.|
|`prev(e)`|有向辺 e の前の有向辺を返す. この順というのは反時計回りに多角形面の辺を辿る順を指す.|
|`incident_face(e)`|有向辺 e が面している面番号を返す.|
|`component_e(f)`|面 f を構成する辺の一つを返す.|
|`area2(f)`|面 f の面積の二倍を返す. 面 f が有界でないなら値は負になる.|
|`area(f)`|面 f の面積を返す. 面 f が有界でないなら値は負になる.|
|`is_outside(f)`|面 f が有界でないならtrueを返す.|

## 参考
[https://en.wikipedia.org/wiki/Doubly_connected_edge_list](https://en.wikipedia.org/wiki/Doubly_connected_edge_list)

