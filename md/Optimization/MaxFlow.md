---
title: 最大流 (Max Flow)
documentation_of: ../../src/Optimization/MaxFlow.hpp
---

## 概要

最大流問題を解くためのライブラリ。Dinic法やPush-Relabel法などのアルゴリズムをテンプレートで切り替えて使用できる。

- `MaxFlow`: 基本的な最大流問題を扱う。
- `MaxFlowLowerBound`: 各辺に最小流量制約がある最大流問題を扱う。

内部アルゴリズムとして以下を選択できる。
- `Dinic`: 計算量 $O(V^2 E)$。二部マッチングなど、特殊なグラフでは $O(E \sqrt{V})$。
- `PushRelabel`: 計算量 $O(V^2 \sqrt{E})$。

---

## 使い方

### 基本的な最大流

`MaxFlow<FlowAlgo>` のテンプレート引数 `FlowAlgo` に、`Dinic<flow_t>` や `PushRelabel<flow_t>` のように内部アルゴリズムと流量の型を指定する。

```cpp
#include <iostream>
#include "src/Optimization/MaxFlow.hpp"

int main() {
    int N = 4, M = 5;
    // 頂点数 N=4 のグラフを、Dinic法（流量はlong long）で構築
    MaxFlow<Dinic<long long>> graph(N);

    // 辺を追加
    graph.add_edge(0, 1, 10);
    graph.add_edge(0, 2, 2);
    graph.add_edge(1, 2, 6);
    graph.add_edge(1, 3, 6);
    graph.add_edge(2, 3, 8);

    // 始点 0 から終点 3 への最大流を計算
    long long flow = graph.maxflow(0, 3);
    std::cout << flow << std::endl; // 12
}
```

---

## `MaxFlow` のインターフェース

### テンプレートパラメータ
- `FlowAlgo`: `Dinic<flow_t>` や `PushRelabel<flow_t>` のように、使用するアルゴリズムと流量の型 `flow_t` を指定する。

### コンストラクタ
- `MaxFlow(size_t n = 0)`: 頂点数 `n` でグラフを初期化する。

### メンバ関数
- `int add_vertex()`: グラフに新しい頂点を1つ追加し、その頂点番号を返す。
- `vector<int> add_vertices(size_t size)`: `size` 個の新しい頂点を追加し、それらの頂点番号を `vector` で返す。
- `EdgePtr add_edge(int src, int dst, flow_t cap, bool bidir = false)`:
  - 頂点 `src` から `dst` へ容量 `cap` の辺を追加する。
  - `bidir` が `true` の場合、`dst` から `src` へも同じ容量の辺が追加され、双方向に流せる辺となる。
  - 追加した辺を操作するための `EdgePtr` を返す。
- `flow_t maxflow(int s, int t)`: 始点 `s` から終点 `t` への最大流を計算して返す。
- `flow_t maxflow(int s, int t, flow_t flow_lim)`: 流せる量の上限を `flow_lim` に制限して最大流を計算する。
- `vector<bool> mincut(int s)`: 最小カットを求める。`s` 側（到達可能）の頂点か否かを `true`/`false` で表す `vector` を返す。

### `EdgePtr` クラス
`add_edge` の返り値。辺の情報を取得したり、容量を変更したりできる。
- `int src()`: 辺の始点を返す。
- `int dst()`: 辺の終点を返す。
- `flow_t flow()`: この辺の現在の流量を返す。
- `flow_t cap()`: この辺の現在の容量を返す。
- `flow_t change_cap(flow_t new_cap, int s, int t)`:
  - 辺の容量を `new_cap` に変更する。
  - 変更によってフローが容量をオーバーした場合、超過分を押し戻すために `s` と `t` の間で追加のフロー計算を行う。
  - 押し戻しきれなかったフロー量を返す。

---

## `MaxFlowLowerBound` のインターフェース

各辺に最小流量制約と最大流量制約がある場合の最大流を求める。

### 使い方
`MaxFlow` と同様に、内部アルゴリズムと流量の型をテンプレートで指定する。

```cpp
MaxFlowLowerBound<Dinic<long long>> graph(N);
graph.add_edge(u, v, lower, upper); // 最小流量 lower, 最大流量 upper の辺を追加
long long flow = graph.maxflow(s, t);
if (flow == -1) {
    // 最小流量制約を満たすフローが存在しない
}
```

### メンバ関数
- `EdgePtr add_edge(int src, int dst, flow_t lower, flow_t upper)`: 頂点 `src` から `dst` へ、最小流量 `lower`、最大流量（容量） `upper` の辺を追加する。
- `flow_t maxflow(int s, int t)`: 始点 `s` から終点 `t` への最大流を計算する。**最小流量制約を満たす有効なフロー（循環流）が存在しない場合、-1を返す。**

## 参考
- [Dinic法の計算量](https://misawa.github.io/others/flow/dinic_time_complexity.html)
