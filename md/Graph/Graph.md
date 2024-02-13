---
title: グラフ
documentation_of: ../../src/Graph/Graph.hpp
---

## 使い方

**構築例**
```c++
int n,m;cin>>n>>m; // n: 頂点数, m: 辺数
Graph g(m);
for(int i=0;i<m;++i)cin>>g[i],--g[i];
g.build(n, 0); // 無向グラフ
// g.build(n, 1); // 有向グラフ
```

**グラフ探索例**
```c++
for(int v=0;v<n;++v)
 for(int e: g(v)){ // 辺リスト
  int u=g[e]-v; // 隣接頂点
  /* do something */
 }
```

## `Edge` クラス
辺 $e=(s,d)$ を表すクラス．

|メンバ変数|概要|
|---|---|
|`s`|辺の端点の一つ．有向辺なら始点を表す．|
|`d`|辺の端点の一つ．有向辺なら終点を表す．|

|||
|---|---|
|`operator--()`| 辺の端点をデクリメント. (1-index の入力をスムーズに 0-indexにするために用意).|
|`operator-(Edge e, int v)`|辺 $e$ の端点のうち頂点 $v$ でない方を返す.|
|`operator>>(istream& is,Edge &e)`| `s d` のフォーマットの入力に対応.  |


## `Graph` クラス

`vector<Edge>` を継承.\
コンストラクタも `vector` . \
辺の配列と隣接リストの役割を担ったクラス.

|名前|概要|
|---|---|
|`build(direct)`|隣接リストを構築する. <br> 引数が `false` なら辺を無向辺とみなす.<br> 引数が `true` なら辺を有向辺とみなす.|
|`vertex_size()`|頂点の数を返す. <br> **※`build` 関数が実行済みであることを前提とする**　|
|`edge_size()`|辺の数を返す.|
|`operator()(u)`| 頂点 $u$ から出る辺 (の番号) のリストを返す. <br> 返り値は [`ListRange<int>`](../Internal/ListRange.hpp). <br> **※`build` 関数が実行済みであることを前提とする**　|