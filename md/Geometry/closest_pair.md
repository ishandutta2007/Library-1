---
title: 最近点対
documentation_of: ../../src/Geometry/closest_pair.hpp
---

分割統治+枝刈り
## 関数

|名前|概要|計算量|
|---|---|---|
|`closest_pair(ps)`| 引数は `vector<Point<K>>`. <br>点群のペアのうち2点間の距離が最も近いもの (最近点対) を返す. |$\mathcal{O}(n \log n)$ <br> ただし引数の点の個数を $n$ とした|

## 参考