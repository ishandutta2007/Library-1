---
title: 二点連結成分分解 (Block-Cut Tree)
documentation_of: ../../src/Graph/block_cut_tree.hpp
---

無向グラフを二点連結成分分解し、その構造を木で表現する Block-Cut Tree を構築する。

![bct.svg](https://github.com/hashiryo/Library/blob/master/img/bct.drawio.svg?raw=true)
<center>図：左が元グラフ、右が構築されたBlock-Cut Tree。丸が元頂点、四角が二点連結成分（ブロック）を表す頂点。</center>

|関数名|概要|計算量|
|---|---|---|
|`block_cut_tree(CSRArray<int> adj)` <br> `block_cut_tree(Graph g)` |無向グラフ g を二点連結成分分解して構築した Block-Cut Tree を返す. <br> 引数は頂点 → 頂点の隣接リスト([`CSRArray<int>`クラス](../Internal/ListRange.hpp)) もしくは [`Graph`クラス](Graph.hpp) で無向グラフを渡す.　<br> 返り値は `Graph` クラス．|$O(V+E)$ |

### Block-Cut Tree の構造

頂点数 $N$ の無向グラフ $g$ の二点連結成分の個数を $C$ とする。
構築される Block-Cut Tree は頂点数が $N+C$ の森である。

- **元の頂点 (Art-Point)**: `0` から `N-1` の頂点が対応する。
  - Block-Cut Tree 上で、元頂点に隣接するのはブロック頂点のみである。
  - 次数が 2 以上の元頂点は、元グラフの**関節点** (articulation point) にあたる。
- **ブロック頂点 (Block-Node)**: `N` から `N+C-1` の頂点が対応する。
  - 各頂点は、元グラフの各二点連結成分（ブロック）を表す。
  - ブロック頂点に隣接するのは、そのブロックに属する元頂点のみである。
- **橋**: 元グラフの橋は、2つの元頂点と1つのブロック頂点からなるパスとして表現される。
- **孤立点**: 元グラフの孤立点は、次数0の元頂点として表現される。

### 使い方

```cpp
// 頂点数N, 辺数Mのグラフgを構築
Graph g(N, M);
for (int i = 0; i < M; ++i) {
    int u, v;
    std::cin >> u >> v;
    g.add_edge(u, v);
}

// Block-Cut Treeを構築
Graph bct = block_cut_tree(g);

// 各二点連結成分（ブロック）に属する頂点を列挙
int num_blocks = bct.vertex_size() - N;
for (int i = 0; i < num_blocks; ++i) {
    int block_node = N + i;
    std::cout << "Block " << i << ":";
    for (const auto& edge : bct.adj(block_node)) {
        int vertex_node = edge.to;
        std::cout << " " << vertex_node;
    }
    std::cout << std::endl;
}
```

## 参考
- [noshi91さんのツイート](https://twitter.com/noshi91/status/1529858538650374144)
- [二点連結成分分解 - なちあの学習帳](https://nachiavivias.github.io/cp-library/column/2022/01.html)
