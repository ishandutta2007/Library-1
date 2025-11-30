---
title: Segment Tree (on Patricia Trie)
documentation_of: ../../src/DataStructure/SegmentTree_Patricia.hpp
---

## 概要

キーのビット表現を利用したパトリシア木 (Patricia Trie) 上に構築されたセグメント木です。
座標圧縮が不要なスパースセグメント木として利用できます。インデックスの最大値 `1LL << HEIGHT` まで扱えますが、実際に `set` されたキーについてのみノードが生成されるため、メモリ効率が良い場合があります。

最大の特徴は、全てのキー `k` を `k XOR bias` に変換した上で区間積や探索を行う `bias` 引数です。これにより、集合内の全要素とある値 `x` とのXORの最小値を求める、といったクエリを高速に処理できます。

また、テンプレートパラメータ `persistent` を `true` に設定することで、永続データ構造として利用できます。

## テンプレートパラメータ

| 名前         | 説明                                                                                                                              | デフォルト値 | 
| ------------ | --------------------------------------------------------------------------------------------------------------------------------- | ------------ | 
| `M`          | 要素のモノイド。`T` (型), `ti()` (単位元), `op(T, T)` (二項演算) を持つ構造体を指定する。指定しない場合、単純な点更新・点取得のみ可能。 | -            | 
| `persistent` | `true` の場合、永続データ構造として動作する。`set` や `at` 操作は新しい根を生成して返す。                                           | `false`      | 
| `HEIGHT`     | 木の高さを指定する。キーとして `[0, 1LL << HEIGHT)` の範囲を扱うことができる。                                                      | `31`         | 

## 計算量

- `set`, `get`, `prod`, `find_first`, `find_last`: $O(\text{HEIGHT})$

## インターフェース

### コンストラクタ

| 名前                                        | 説明                                                     | 
| ------------------------------------------- | -------------------------------------------------------- | 
| `SegmentTree_Patricia()`                      | 空の木を構築する。                                       | 
| `SegmentTree_Patricia(size_t n, T val)`       | `[0, n)` の範囲を `val` で初期化する。                   | 
| `SegmentTree_Patricia(T* bg, T* ed)`          | `[bg, ed)` の範囲の配列で初期化する。                     | 
| `SegmentTree_Patricia(const vector<T>& ar)` | `ar` の内容で初期化する。                                | 

### メソッド

| 名前                                      | 説明                                                                                                                                        | 
| ----------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------- | 
| `void set(id_t k, T val)`                 | `k` 番目の要素を `val` に更新する。                                                                                                         | 
| `T get(id_t k)`                           | `k` 番目の要素を取得する。                                                                                                                  | 
| `T& at(id_t k)`                           | `k` 番目の要素への参照を取得する (モノイド未指定時のみ)。                                                                                   | 
| `T prod(id_t a, id_t b, id_t bias = 0)`   | `[a, b)` の区間積を計算する。`bias` を指定すると、各キー `k` を `k XOR bias` として計算する。                                                 | 
| `find_first(id_t a, C check, id_t bias=0)`| `[a, 1LL<<HEIGHT)` の範囲で `check(prod(a, i+1))` が `true` となる最小の `i` を探す。`bias` は`prod`と同様。見つからない場合は `-1` を返す。 | 
| `find_last(id_t b, C check, id_t bias=0)` | `[0, b)` の範囲で `check(prod(i, b))` が `true` となる最大の `i` を探す。`bias` は`prod`と同様。見つからない場合は `-1` を返す。         | 

## 使用例

### 1. スパースな区間和 (Range Sum)

```cpp
struct RsumQ {
  using T = int;
  static T ti() { return 0; }
  static T op(const T &l, const T &r) { return l + r; }
};

SegmentTree_Patricia<RsumQ, false, 60> seg; // 高さ60 (約10^18まで)
seg.set(12345, 10);
seg.set(9876543210, 20);
// [0, 100000) の和を求める
cout << seg.prod(0, 100000) << endl; // 10
```

### 2. 集合のXOR最小値

> 問題: 整数からなる集合 `S` がある。`Q` 個のクエリを処理せよ。
> - `0 x`: `S` に `x` を追加
> - `1 x`: `S` から `x` を削除
> - `2 x`: `S` の要素 `y` と `x` のXOR `y XOR x` の最小値を求めよ

`find_first` と `bias` を使うことで、この問題を効率的に解くことができます。
`S` に含まれる整数 `y` の位置を `1`、他を `0` としたセグメント木を考えます。
`y XOR x` の最小値を求めることは、`bias = x` を設定した `find_first` で最初の `1` を探すことと同じです。

```cpp
#include <iostream>
#include "src/DataStructure/SegmentTree_Patricia.hpp"

struct RsumQ {
  using T = int;
  static T ti() { return 0; }
  static T op(const T &l, const T &r) { return l + r; }
};

int main() {
  std::cin.tie(0);
  std::ios::sync_with_stdio(0);
  SegmentTree_Patricia<RsumQ> S;
  auto check = [&](int sum) { return sum >= 1; };
  int Q;
  std::cin >> Q;
  while (Q--) {
    int op, x;
    std::cin >> op >> x;
    if (op == 0) {
      if (S.get(x) == 0) S.set(x, 1); // 既に追加済みでなければ追加
    } else if (op == 1) {
      if (S.get(x) == 1) S.set(x, 0); // 含まれていれば削除
    } else {
      // check を満たす最初のインデックスを探す
      // bias=x を与えているので、インデックスkは k XOR x として扱われる
      // したがって、これは (y XOR x) の最小値を探すのと同じ
      std::cout << S.find_first(0, check, x) << '\n';
    }
  }
  return 0;
}
```

## Verify

- [Codeforces Round #470 (rated, Div. 1, based on VK Cup 2018 Round 1) C. Perfect Security](https://codeforces.com/contest/947/problem/C) (find+xor)
- [Codeforces Round #477 (rated, Div. 1, based on VK Cup 2018 Round 3) C. Big Secret](https://codeforces.com/contest/966/problem/C) (find+xor)
- [Codeforces Round #179 (Div. 1) E. Yaroslav and Points](https://codeforces.com/contest/295/problem/E) (特殊モノイド+座圧サボり)
- [Kyoto University Programming Contest 2018 M - 整数と根付き木](https://atcoder.jp/contests/kupc2018/tasks/kupc2018_m) (パトリシアの効力発揮)
- [CodeChef Gotham PD](https://www.codechef.com/problems/GPD) (永続, xor)
- [CodeChef Pishty and tree](https://www.codechef.com/problems/PSHTTR) (永続)
- [Yosupo Judge - Set XOR Min](https://judge.yosupo.jp/problem/set_xor_min) (find+xor)