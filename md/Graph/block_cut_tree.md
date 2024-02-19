---
title: 二点連結成分分解 (block-cut-tree (拡張))
documentation_of: ../../src/Graph/block_cut_tree.hpp
---
![bct.svg](https://github.com/hashiryo/Library/blob/master/img/bct.drawio.svg?raw=true)

|関数名|概要|計算量|
|---|---|---|
|`block_cut_tree(CSRArray<int> adj)` <br> `block_cut_tree(Graph g)` |無向グラフ g を二点連結成分分解して構築した block-cut-tree (拡張) を返す. <br> 引数は頂点 → 頂点の隣接リスト([`CSRArray<int>`クラス](../Internal/ListRange.hpp)) もしくは [`Graph`クラス](Graph.hpp) で無向グラフを渡す.　<br> 返り値は `Graph` クラス．|$O(V+E)$ |


### block-cut-tree (拡張) について
[参考](https://twitter.com/noshi91/status/1529858538650374144?s=20&t=eznpFbuD9BDhfTb4PplFUg)

頂点数 $N$ の無向グラフ $g$ の二点連結成分の個数を $C$ とする.\
block-cut-tree (拡張) は頂点数が $N+C$ の森である. 
- $N$ 個の頂点 $0,1,\dots,N-1$ は元のグラフ $g$ の頂点に対応.
  - 隣接頂点は属する二点連結成分を指す.
  - 隣接頂点が複数  $(>1)\rightarrow$ 関節点
- $C$ 個の頂点 $N,N+1,\dots,N+C-1$ は $g$ の二点連結成分に対応.
  - 隣接頂点はその二点連結成分を構成する頂点を指す.

## 問題例
[AtCoder Regular Contest 062 F - AtCoDeerくんとグラフ色塗り](https://atcoder.jp/contests/arc062/tasks/arc062_d)
## 参考
[https://twitter.com/noshi91/status/1529858538650374144?s=20&t=eznpFbuD9BDhfTb4PplFUg](https://twitter.com/noshi91/status/1529858538650374144?s=20&t=eznpFbuD9BDhfTb4PplFUg)\
[https://nachiavivias.github.io/cp-library/column/2022/01.html](https://nachiavivias.github.io/cp-library/column/2022/01.html)