---
title: 二部グラフの辺彩色
documentation_of: ../../src/Graph/bipartite_edge_coloring.hpp
---

|関数名|概要|計算量|
|---|---|---|
|`bipartite_edge_coloring(bg)`|二部グラフの辺彩色を構築する．<br> 引数は [`BipartiteGraph` クラス](BipartiteGraph.hpp)．<br> 戻り値は辺のサイズの `vector<int>` で各辺への色の割り当てを表す．| <br>$O(E\sqrt{V}\log \Delta)$ <br> ただし頂点の次数のうち最大のものを $\Delta$ とおいた． |


## 問題例
[AtCoder Grand Contest 037 D - Sorting a Grid](https://atcoder.jp/contests/agc037/tasks/agc037_d) (sp judge)

## 参考
[https://ei1333.hateblo.jp/entry/2020/08/25/015955](https://ei1333.hateblo.jp/entry/2020/08/25/015955)