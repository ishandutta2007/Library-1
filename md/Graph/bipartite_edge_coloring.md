---
title: 二部グラフの辺彩色
documentation_of: ../../src/Graph/bipartite_edge_coloring.hpp
---

|関数名|概要|計算量|
|---|---|---|
|`bipartite_edge_coloring(bg)`|二部グラフの辺彩色を構築する．<br> 引数は [`BipartiteGraph` クラス](BipartiteGraph.hpp)．<br> 戻り値は辺数と同じサイズの `vector<int>` で，各辺への色の割り当てを表す．頂点の最大次数を $\Delta$ とすると、 $\Delta$ 色で彩色され，色は $0, 1, \dots, \Delta-1$ で表現される．| <br>$O(E\sqrt{V}\log \Delta)$ <br> ただし $\Delta$ は頂点の最大次数． |


## Verify


- [AtCoder Grand Contest 037 D - Sorting a Grid](https://atcoder.jp/contests/agc037/tasks/agc037_d) (sp judge)



## 参考
[https://ei1333.hateblo.jp/entry/2020/08/25/015955](https://ei1333.hateblo.jp/entry/2020/08/25/015955)