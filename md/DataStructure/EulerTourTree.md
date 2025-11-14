---
title: Euler-Tour-Tree
documentation_of: ../../src/DataStructure/EulerTourTree.hpp
---

## 概要
Euler Tour Tree は、木を動的に管理するためのデータ構造である。以下の操作を効率的に行うことができる。

- 2つの木を辺で連結する (`link`)
- 木から辺を削除して2つの木に分割する (`cut`)
- 2頂点が同じ木に属するか（連結か）を判定する (`connected`)
- 頂点の値の更新 (`set`)
- 部分木に対する集約クエリ（総和、最大値など） (`prod_subtree`)
- 部分木に対する区間更新 (`apply_subtree`)

実装には Splay Tree を用いており、各操作は償却計算量 $O(\log N)$ で動作する。

## テンプレートパラメータ

- `typename M`: 頂点の値に乗せるモノイドや、作用素を指定する構造体。詳細は後述。
- `size_t NODE_SIZE`: Euler Tour Tree が内部で確保するノードの最大数。頂点数 $N$ と追加される辺の最大数を考慮して設定する必要がある。デフォルトは `4,000,000` で、おおよそ $N \le 2^{20}$ に対応する。

### モノイド `M` の要件

`M` に指定する構造体には、用途に応じて以下の型や静的メンバ関数を定義する必要がある。

#### 頂点に対する集約クエリ (例: 部分木の総和)

- `using T`: 頂点の値の型。
- `static T ti()`: モノイドの単位元を返す関数。
- `static T op(T l, T r)`: 2つの値を併合する二項演算。

#### 部分木に対する区間更新 (例: 部分木の値に一様加算)

- `using E`: 作用素の型。
- `static void mp(T &v, E x)`: 頂点の値 `v` に作用素 `x` を適用する関数。
- `static void cp(E &pre, E suc)`: 作用素 `pre` に `suc` を合成する関数。

`mp` は、`mp(T &v, E x, int sz)` のように第三引数として部分木のサイズを受け取るオーバーロードも定義できる。

## API (主要なメンバ関数)

- `EulerTourTree(int N)`: 頂点数 `N` の森を初期化する。各頂点は自身のみからなる木を構成する。
- `void link(int u, int v)`: 頂点 `u` と `v` の間に辺を張り、木を連結する。`u` と `v` が既に連結である場合は未定義動作。
- `void cut(int u, int v)`: 頂点 `u` と `v` の間の辺を削除し、木を分割する。`u` と `v` の間に辺がない場合は未定義動作。
- `bool connected(int u, int v)`: 頂点 `u` と `v` が同じ木に属しているかを判定する。
- `void set(int v, T val)`: 頂点 `v` の値を `val` に更新する。
- `const T& get(int v)`: 頂点 `v` の値を取得する。`ett[v]` としてもアクセス可能。
- `T prod_subtree(int v, int p)`: `p` を親としたときの頂点 `v` の部分木全体に対するモノイド積を返す。
- `void apply_subtree(int v, int p, E val)`: `p` を親としたときの頂点 `v` の部分木全体に作用素 `val` を適用する。
- `size_t tree_size(int v)`: 頂点 `v` が属する木のサイズ（頂点数）を返す。

## 使用例

### 部分木の頂点の値の和を求める

```cpp
#include <iostream>
#include "src/DataStructure/EulerTourTree.hpp"

// モノイドを定義する構造体
// 部分木の総和を求める
struct RsumQ {
    using T = long long;
    static T ti() { return 0; }
    static T op(T l, T r) { return l + r; }
};

int main() {
    int N, Q;
    std::cin >> N >> Q;

    EulerTourTree<RsumQ> ett(N);

    // 初期値の設定
    for (int i = 0; i < N; ++i) {
        long long a;
        std::cin >> a;
        ett.set(i, a);
    }

    // 木の構築
    for (int i = 0; i < N - 1; ++i) {
        int u, v;
        std::cin >> u >> v;
        ett.link(u, v);
    }

    for (int q = 0; q < Q; ++q) {
        int type;
        std::cin >> type;
        if (type == 0) {
            // 辺 (u, v) を削除し、辺 (w, x) を追加
            int u, v, w, x;
            std::cin >> u >> v >> w >> x;
            ett.cut(u, v);
            ett.link(w, x);
        } else if (type == 1) {
            // 頂点 p の値に x を加算
            int p;
            long long x;
            std::cin >> p >> x;
            ett.set(p, ett.get(p) + x);
        } else {
            // p を親としたときの v の部分木の総和を求める
            int v, p;
            std::cin >> v >> p;
            std::cout << ett.prod_subtree(v, p) << '\n';
        }
    }

    return 0;
}
```

## 計算量
各操作の償却計算量は $O(\log N)$ である。
