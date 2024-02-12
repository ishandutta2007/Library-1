---
title: グラフ
documentation_of: ../../src/Graph/BiConnectedComponents.hpp
---

## 使い方

**構築例**
```c++
int n,m;cin>>n>>m; // n: 頂点数, m: 辺数
Graph g(n);
for(int i=0,u,v;i<m;++i)cin>>u>>v, g.add_edge(--u,--v);
g.build(1); // 無向グラフ
// g.build(0); // 有向グラフ
```

**グラフ探索例**
```c++
for(int v=0;v<n;++v)
 for(int e: g(v)){ // 辺リスト
  int u=g[e]-v; // 隣接頂点
  /* do something */
 }
```

## `Graph::Edge` クラス
辺 $e=(s,d)$ を表すクラス．

|メンバ変数|概要|
|---|---|
|`s`|辺の端点の一つ．有向辺なら始点を表す．|
|`d`|辺の端点の一つ．有向辺なら終点を表す．|

|||
|---|---|
|`operator-(Edge e, int v)`|辺 $e$ の端点のうち頂点 $v$ でない方を返す.|


## `Graph` クラス

辺の配列と隣接リストの役割を担ったクラス. \
**※`add_edge` (とコンストラクタと`build`) 以外の関数は `build` 関数が実行済みであることを前提とする**　\
頂点は $0,\dots, n-1$ でラベル付けする.

|名前|概要|
|---|---|
|`Graph(n)`|コンストラクタ. 頂点数 $n$ を与える.|
|`add_edge(u,v)`|辺 $(u,v)$ を追加する. <br> 有向辺なら 第一引数を始点, 第二引数を終点とする.|
|`build(undirect)`|隣接リストを構築する. <br> 引数が `true` なら追加された辺を無向辺とみなす.<br> 引数が `false` なら追加された辺を有向辺とみなす.|
|`vertex_size()`|頂点の数を返す.|
|`edge_size()`|辺の数を返す.|
|`Edge& operator[](e)` <br> `const Edge& operator[](e)` | $e$ 番目の辺を返す. <br> 返り値は `Edge` クラスの参照.|
|`begin()`|辺の配列のイテレータ.|
|`end()`|辺の配列のイテレータ.|
|`operator()(u)`| 頂点 $u$ から出る辺 (の番号) のリストを返す. <br> 返り値は [`ListRange<int>`](src/Internal/ListRange.hpp). |