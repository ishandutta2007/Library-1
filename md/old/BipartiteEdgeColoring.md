---
title: 二部グラフの辺彩色
documentation_of: ../../src/old/BipartiteEdgeColoring.hpp
---
明示的に二部グラフを表現する \
頂点の添字は左右それぞれ [0,L), [0,R) で表現
## メンバ関数

| 名前                         | 概要                                                              | 計算量                                                                           |
| ---------------------------- | ----------------------------------------------------------------- | -------------------------------------------------------------------------------- |
| `BipartiteEdgeColoring(L,R)` | コンストラクタ. 二部グラフの左右のサイズ L, Rを渡す               |                                                                                  |
| `add_edge(l,r)`              | 辺 (l,r) を追加                                                   |                                                                                  |
| `edge_coloring()`            | 辺彩色をした結果を返す. 辺は `add_edge` をした順で番号付けをする. | $\mathcal{O}(E\sqrt{V}\log \Delta)$ <br> ただし $\Delta = \max_{v\in V} \deg(v)$ |


## 問題例
[AtCoder Grand Contest 037 D - Sorting a Grid](https://atcoder.jp/contests/agc037/tasks/agc037_d) (sp judge)

## 参考
[https://ei1333.hateblo.jp/entry/2020/08/25/015955](https://ei1333.hateblo.jp/entry/2020/08/25/015955)