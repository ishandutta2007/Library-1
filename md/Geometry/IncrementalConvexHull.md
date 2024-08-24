---
title: 削除なし動的凸包
documentation_of: ../../src/Geometry/IncrementalConvexHull.hpp
---

## メンバ関数

|名前|概要|計算量|
|---|---|---|
|`IncrementalConvexHull()`|デフォルトコンストラクタ||
|`edge_size()`|凸包の辺の本数を返す.|$O(1)$|
|`area()`|凸包の面積を返す.|$O(1)$|
|`area2()`|凸包の面積の2倍を返す. <br>これは整数のまま扱うことを想定したもの.|$O(1)$|
|`where(p)`| 点 $\boldsymbol{p}$ と凸包との位置関係を表す. <br> +1: 凸包の内部, 0: 凸包の周上, -1: 凸包の外部|$O(\log n)$|
|`insert(p)`| 点 $\boldsymbol{p}$ を含むように凸包を再構成する.|$O(\log n)$|

## 参考
[https://maspypy.github.io/library/geo/incremental_convexhull.hpp](https://maspypy.github.io/library/geo/incremental_convexhull.hpp)