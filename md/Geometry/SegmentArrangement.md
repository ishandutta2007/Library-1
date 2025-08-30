---
title: 線分アレンジメント
documentation_of: ../../src/Geometry/SegmentArrangement.hpp
---

## 概要
二次元平面上に与えられた複数の線分について、それらの端点および線分同士の交点を頂点とするグラフ構造（平面グラフ）を構築する。このデータ構造は線分アレンジメントと呼ばれ、Doubly Connected Edge List (DCEL) を用いて表現される。

これにより、各頂点、辺、面に関する情報を効率的に取得できる。例えば、ある頂点に接続する辺を列挙したり、ある面を構成する辺を反時計回りに走査したりすることが可能になる。

### 計算量
- コンストラクタ: $O(N^2 + V \log V)$
  - $N$: 入力する線分の数
  - $V$: 構築されるグラフの頂点数（線分の端点と交点の総数）

## `SegmentArrangement<K>` クラス
### テンプレートパラメータ
- `K`: 座標の型（整数型など）

### メンバ関数
|メンバ関数|概要|
|---|---|
|`SegmentArrangement(ss)`|コンストラクタ. `vector<Segment<K>>` を引数にとる. 線分アレンジメントを実行. | 
|`vertex_size()`|頂点数を返す。|
|`face_size()`|面数を返す。|
|`out_edges(v)`|頂点 `v` から出る有向辺のリストを返す。|
|`point(v)`|頂点 `v` に対応する二次元平面上の点 (`Point<K>`) を返す。|
|`vertex(p)`|二次元平面上の点 (`Point<K>`) $\boldsymbol{p}$ に対応する頂点番号を返す。|
|`to_v(e)`| 有向辺 `e` の終点の頂点を返す。|
|`from_v(e)`|有向辺 `e` の始点の頂点を返す。|
|`twin(e)`|有向辺 `e` と同じ辺で向きを逆にした有向辺を返す。|
|`next(e)`|ある面において、有向辺 `e` の次に反時計回りで現れる辺を返す。|
|`prev(e)`|ある面において、有向辺 `e` の前に反時計回りで現れる辺を返す。|
|`incident_face(e)`|有向辺 `e` が左手に見る面の番号を返す。|
|`component_e(f)`|面 `f` の境界を構成する有向辺のいずれか一つを返す。|
|`area2(f)`|面 `f` の有向面積の2倍を返す。面が反時計回りの頂点順で定義されていれば正、時計回りなら負となる。|
|`area(f)`|面 `f` の有向面積を返す。|
|`is_outside(f)`|面 `f` が無限面（最も外側の面）であれば `true` を返す。これは有向面積が負になるかで判定される。|

## 使用例
```cpp
#include <iostream>
#include <vector>
#include "src/Geometry/SegmentArrangement.hpp"

int main() {
    // 2つの線分 {(0,0)-(2,2)}, {(0,2)-(2,0)} を定義
    std::vector<geo::Segment<long long>> ss = {
        {{0, 0}, {2, 2}},
        {{0, 2}, {2, 0}}
    };

    geo::SegmentArrangement sa(ss);

    // 頂点数: 5 ( (0,0), (0,2), (1,1), (2,0), (2,2) )
    std::cout << "Vertices: " << sa.vertex_size() << std::endl;
    // 面数: 5 (中央のひし形と周りの4つの領域 + 無限面1つ)
    std::cout << "Faces: " << sa.face_size() << std::endl;

    // 各頂点の座標を出力
    for (size_t i = 0; i < sa.vertex_size(); ++i) {
        std::cout << "Vertex " << i << ": " << sa.point(i) << std::endl;
    }
}
```

## 参考
- [Doubly connected edge list - Wikipedia](https://en.wikipedia.org/wiki/Doubly_connected_edge_list)