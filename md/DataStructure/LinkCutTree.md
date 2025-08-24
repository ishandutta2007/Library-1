---
title: Link-Cut-Tree
documentation_of: ../../src/DataStructure/LinkCutTree.hpp
---

## 概要
Link-Cut Tree は木の集合を管理するデータ構造。以下の操作を効率的に行うことができる。
- `link(u, v)`: ２つの木に属する頂点 u, v を辺で結ぶ。u は v の子となる。
- `cut(u, v)`: 頂点 u, v を結ぶ辺を切り離す。
- `evert(u)`: 頂点 u を全体の根にする。
- パスに対するクエリ: `prod(u, v)` で u-v パスの集約値、`apply(u, v, x)` で u-v パスへの作用を適用できる。
- `lca(u, v)`: 最小共通祖先を求める。

## テンプレートパラメータ
`LinkCutTree<M>` のように、作用素モノイド `M` をテンプレートパラメータとして渡すことで、ノードが持つ値の型や、パスに対する操作をカスタマイズできる。

`M` は以下のメンバを持つ構造体として定義する。

- `using T = ...`: ノードが持つ値の型
- `using E = ...`: パスへの作用素の型
- `static T op(const T &l, const T &r)`: 値のマージ演算。`prod` で使われる。
- `static void mp(T &v, const E &f)`: 値 `v` に作用素 `f` を適用する。`apply` で使われる。
- `static void cp(E &f, const E &g)`: 作用素 `f` に作用素 `g` を合成する。`apply` で使われる。
- `using commute = void`: `op` が可換である場合に定義する。

## 計算量
各操作のならし計算量は $O(\log n)$ である。

## インターフェース
- `LinkCutTree(size_t sz)`: `sz` 個の頂点を持つ森を構築する。
- `LinkCutTree(size_t sz, T val)`: `sz` 個の頂点を持つ森を構築し、全頂点の値を `val` で初期化する。
- `evert(int k)`: 頂点 `k` を根にする。
- `link(int c, int p)`: `c` の親が `p` となるように辺を張る。
- `cut(int c, int p)`: `c` と親 `p` を結ぶ辺を削除する。
- `root(int x)`: `x` が属する木の根を返す。
- `parent(int x)`: `x` の親を返す。根の場合は `-1` を返す。
- `lca(int x, int y)`: `x` と `y` の最小共通祖先を返す。同じ木に属さない場合は `-1` を返す。
- `get(int k)` / `operator[](int k)`: 頂点 `k` の値を取得する。
- `set(int k, const T &v)`: 頂点 `k` の値を `v` に設定する。
- `prod(int a, int b)`: パス `a-b` 上の頂点の値の集約結果（`op`による）を返す。
- `apply(int a, int b, const E &v)`: パス `a-b` 上の各頂点に作用 `v` を適用する。

## 使用例
パス上のノードの値（long long）の和を求める例。

```cpp
#include "src/DataStructure/LinkCutTree.hpp"
#include <iostream>

// モノイドを定義
struct RsumQ {
    using T = long long;
    static T op(const T &l, const T &r) { return l + r; }
    // op は可換
    using commute = void;
};

int main() {
    int N = 5;
    LinkCutTree<RsumQ> lct(N);

    // 初期値を設定
    for (int i = 0; i < N; ++i) {
        lct.set(i, i + 1);
    }

    // 木の構造を定義
    // 1 -- 0 -- 2
    //      |
    //      3 -- 4
    lct.link(1, 0);
    lct.link(2, 0);
    lct.link(3, 0);
    lct.link(4, 3);

    // パス 1-4 の和を計算 (1-0-3-4)
    // 1-0-3-4 の値は 2, 1, 4, 5
    // 2 + 1 + 4 + 5 = 12
    long long sum = lct.prod(1, 4);
    std::cout << "Sum on path 1-4: " << sum << std::endl; // 12

    // 頂点 0 の値を 10 に更新
    lct.set(0, 10);

    // 再度パス 1-4 の和を計算
    // 1-0-3-4 の値は 2, 10, 4, 5
    // 2 + 10 + 4 + 5 = 21
    sum = lct.prod(1, 4);
    std::cout << "Sum on path 1-4 after update: " << sum << std::endl; // 21
}
```
