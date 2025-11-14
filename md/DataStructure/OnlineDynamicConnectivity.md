---
title: Online-Dynamic-Connectivity
documentation_of: ../../src/DataStructure/OnlineDynamicConnectivity.hpp
---

## 概要
オンラインでグラフの辺の追加・削除を行いながら、連結性や連結成分ごとの集約値をクエリできるデータ構造。
内部で Euler Tour Tree を利用している。

## テンプレートパラメータ
- `typename M`: 頂点の値に乗せるモノイド。
  - `M::T`: 値の型
  - `M::ti()`: 単位元
  - `M::op(l, r)`: 演算
- `std::size_t NODE_SIZE`: Euler Tour Tree が内部で確保するノードの最大サイズ。デフォルトは `4'000'000`。

## 使い方
`OnlineDynamicConnectivity<M> dicon(N)` のように宣言する。

| 関数 | 説明 | 計算量 |
|---|---|---|
| `dicon.link(u, v)` | 頂点 `u` と `v` の間に辺を追加する。 | $O((\log N)^2)$ |
| `dicon.cut(u, v)` | 頂点 `u` と `v` の間の辺を削除する。 | $O((\log N)^2)$ |
| `dicon.connected(u, v)` | 頂点 `u` と `v` が連結か判定する。 | $O(\log N)$ |
| `dicon.set(k, val)` | 頂点 `k` の値を `val` に設定する。 | $O(\log N)$ |
| `dicon.get(k)` / `dicon[k]` | 頂点 `k` の値を取得する。 | $O(\log N)$ |
| `dicon.prod(k)` | 頂点 `k` が属する連結成分の集約値を取得する。 | $O(\log N)$ |
| `dicon.apply(k, f)` | 頂点 `k` が属する連結成分の各頂点に作用 `f` を適用する。 | $O(\log N)$ |
| `dicon.size(k)` | 頂点 `k` が属する連結成分のサイズを取得する。 | $O(\log N)$ |

## コード例
```cpp
// 連結成分ごとの合計値を管理する例
struct Monoid {
    using T = long long;
    static T ti() { return 0; }
    static T op(T a, T b) { return a + b; }
};

OnlineDynamicConnectivity<Monoid> dicon(N);

// 頂点 0 に値 10 をセット
dicon.set(0, 10);

// 辺 (0, 1) を追加
dicon.link(0, 1);

// 頂点 0 が属する連結成分の合計値を取得
long long sum = dicon.prod(0);
```

## 計算量
- `link`, `cut`: $O((\log N)^2)$
- `connected`, `set`, `get`, `prod`, `apply`, `size`: $O(\log N)$

## 参考
[https://qiita.com/hotman78/items/78cd3aa50b05a57738d4](https://qiita.com/hotman78/items/78cd3aa50b05a57738d4)

## Verify
- [Educational Codeforces Round 62 F. Extending Set of Points](https://codeforces.com/contest/1140/problem/F)
- [AOJ 2235 - Chain Disconnection](https://onlinejudge.u-aizu.ac.jp/problems/2235)
- [AOJ 2893 - To Star a New Journey](https://onlinejudge.u-aizu.ac.jp/problems/2893)
- [Yosupo Judge - Dynamic Graph Vertex Add Component Sum](https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum)