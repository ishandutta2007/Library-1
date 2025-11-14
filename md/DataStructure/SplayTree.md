---
title: Splay-Tree
documentation_of: ../../src/DataStructure/SplayTree.hpp
---

## 概要
Splay Tree は、列（Sequence）を管理するための平衡二分探索木です。
ノードの回転操作（Splay）によって木を再平衡化することで、様々な操作を償却計算量 $O(\log N)$ で実現します。

この実装では、列に対する以下の操作が可能です。
- 要素の挿入、削除、アクセス、更新
- 2つの列の連結（merge）、列の分割（split）
- 区間に対する二項演算の適用結果の取得（`prod`）
- 区間に対する作用素の適用（`apply`）
- 区間反転（`reverse`）

機能はテンプレートパラメータ `M` に定義された内容によって自動で有効化されます。

## 計算量
各操作の計算量は償却 $O(\log N)$ です。

## テンプレートパラメータ
`SplayTree<M, reversible>`

- `M`: Splay Treeのノードが持つデータ型や、データに対する操作を定義する構造体。詳細は後述。
- `reversible`: `bool` 値。`true` に設定すると、区間反転操作 `reverse` が有効になります。

### `M` の構造
`M` 構造体に定義する型や静的メンバ関数によって、Splay Treeの機能が拡張されます。

| 定義            | 説明                                                                                             | 有効になる機能                               |
| --------------- | ------------------------------------------------------------------------------------------------ | -------------------------------------------- |
| `using T = ...` | ノードが持つ値の型 (必須)                                                                        | 基本機能 (insert, erase, split, merge, etc.) |
| `op(T, T)`      | 2つの `T` 型の値を結合する二項演算。半群(semigroup)をなす必要があります。                          | `prod` (区間積)                              |
| `using E = ...` | ノードに作用させる作用素の型                                                                     |                                              |
| `mp(T&, E)`     | `T` 型の値に `E` 型の作用素を適用する関数。`mp(val, act, size)` のように部分木のサイズも引数に取れます。 | `apply` (区間作用)                           |
| `cp(E&, E)`     | `E` 型の作用素同士を合成する関数                                                                 | `apply` (区間作用)                           |
| `using commute` | `op` が可換であることを示す型エイリアス                                                  | `mul` (特定の要素への `op` 適用)             |

#### 使用例：区間アフィン変換・区間和
`yosupo/dynamic_sequence_range_affine_range_sum` での使用例です。
```cpp
using Mint = ModInt<998244353>;
struct RaffineRsumQ {
  // ノードが持つ値の型 (区間和)
  using T = Mint;
  // 作用素の型 (アフィン変換 ax+b)
  using E = array<Mint, 2>;

  // 二項演算 (和)
  static T op(T vl, T vr) { return vl + vr; }
  // 値への作用
  static void mp(T &val, const E &f, int sz) { val = f[0] * val + f[1] * sz; }
  // 作用素の合成
  static void cp(E &pre, const E &suf) { pre[0] *= suf[0], pre[1] = suf[0] * pre[1] + suf[1]; }
};

// 区間反転を有効にして SplayTree を宣言
SplayTree<RaffineRsumQ, true> st;
```

## API リファレンス

### コンストラクタ
| 名前 | 説明 | 計算量 |
|---|---|---|
| `SplayTree()` | 空の Splay Tree を構築します。 | $O(1)$ |
| `SplayTree(size_t n, const T &val)` | `val` で初期化されたサイズ `n` の列を構築します。 | $O(N)$ |
| `SplayTree(const vector<T> &v)` | `std::vector` から Splay Tree を構築します。 | $O(N)$ |

### メンバ関数
| 名前 | 説明 | 計算量 |
|---|---|---|
| `size()` | 列の要素数を返します。 | $O(1)$ |
| `clear()` | 列を空にします。 | $O(1)$ |
| `dump()` | 列の要素を `std::vector` にして返します。 | $O(N)$ |
| `insert(k, val)` | `k` 番目の位置に `val` を挿入します。 | $O(\log N)$ |
| `erase(k)` | `k` 番目の要素を削除します。 | $O(\log N)$ |
| `get(k)` / `operator[](k)` | `k` 番目の要素への参照を返します。 | $O(\log N)$ |
| `set(k, val)` | `k` 番目の要素を `val` に更新します。 | $O(\log N)$ |
| `split(k)` | 列を `[0, k)` と `[k, size())` の2つに分割します。 | $O(\log N)$ |
| `split3(a, b)` | 列を `[0, a)`, `[a, b)`, `[b, size())` の3つに分割します。 | $O(\log N)$ |
| `operator+(rhs)` | 2つの列を連結します。 | $O(\log N)$ |
| `prod(a, b)` | 区間 `[a, b)` の要素全体に二項演算 `op` を適用した結果を返します。 | $O(\log N)$ |
| `apply(a, b, x)` | 区間 `[a, b)` の各要素に作用素 `x` を適用します。 | $O(\log N)$ |
| `reverse(a, b)` | 区間 `[a, b)` の要素の並びを反転します。`reversible=true` の時のみ有効。 | $O(\log N)$ |

## Verify
- [CODE FESTIVAL 2014 エキシビション B - カッコつけ](https://atcoder.jp/contests/code-festival-2014-exhibition/tasks/code_festival_exhibition_b) (insert, erase, prod)
- [SoundHound Programming Contest 2018 Masters Tournament 本戦 (Open) E - Hash Swapping](https://atcoder.jp/contests/soundhound2018-summer-final-open/tasks/soundhound2018_summer_final_e) (split3, prod)
- [Dynamic Sequence Range Affine Range Sum](https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum) (insert, erase, reverse, apply, prod)