---
title: "Mo's Algorithm"
documentation_of: ../../src/Misc/Mo.hpp
---

## 概要
Mo's Algorithm (クエリ平方分割) は、数列に対する多数の区間クエリをオフラインで高速に処理するアルゴリズム。

クエリを特定の順序に並び替えることで、区間の境界を移動させる回数を最適化する。区間の伸縮に伴う状態変化を差分更新できる問題に適用できる。

## API
`Mo` 構造体を提供する。

- `Mo()`: コンストラクタ
- `void query(int l, int r)`: 半開区間 `[l, r)` のクエリを追加する。
- `void run(const AL &add_left, const AR &add_right, const EL &erase_left, const ER &erase_right, const O &out)`: クエリを処理する。
  - `add_left(int index)`: 区間が左に伸びて `index` を含んだときの処理
  - `add_right(int index)`: 区間が右に伸びて `index` を含んだときの処理
  - `erase_left(int index)`: 区間が左に縮んで `index` を除外したときの処理
  - `erase_right(int index)`: 区間が右に縮んで `index` を除外したときの処理
  - `out(int query_index)`: `query()` で `query_index` 番目 (0-indexed) に追加されたクエリに対する結果を求める処理
- `void run(const A &add, const E &erase, const O &out)`: `add_left`, `add_right` が同じ処理、`erase_left`, `erase_right` が同じ処理の場合に使える簡易版。
  - `add(int index)`: 区間に `index` が追加されたときの処理
  - `erase(int index)`: 区間から `index` が除外されたときの処理
  - `out(int query_index)`: `query()` で `query_index` 番目 (0-indexed) に追加されたクエリに対する結果を求める処理

## コード例
数列 `a` に対する区間 `[l, r)` の数値の種類数を求める例。

```cpp
#include <iostream>
#include <vector>
#include "src/Misc/Mo.hpp"

int main() {
    int n;
    std::cin >> n;
    std::vector<int> a(n);
    std::vector<int> counts; // 各数値の出現回数を管理
    int current_variety = 0; // 現在の区間内の数値の種類数

    // 数値の種類を数えるため、最大値+1のサイズを確保
    int max_val = 0;
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
        if (a[i] > max_val) max_val = a[i];
    }
    counts.resize(max_val + 1, 0);

    int q;
    std::cin >> q;
    Mo mo;
    for (int i = 0; i < q; ++i) {
        int l, r;
        std::cin >> l >> r;
        mo.query(l, r);
    }

    auto add = [&](int i) {
        if (counts[a[i]]++ == 0) {
            current_variety++;
        }
    };
    auto erase = [&](int i) {
        if (--counts[a[i]] == 0) {
            current_variety--;
        }
    };

    std::vector<int> ans(q);
    auto out = [&](int i) {
        ans[i] = current_variety;
    };

    mo.run(add, erase, out);

    for (int i = 0; i < q; ++i) {
        std::cout << ans[i] << std::endl;
    }

    return 0;
}
```

## 計算量
$O((N+Q)\sqrt{Q} \cdot T)$

$N$ は数列の長さ、$Q$ はクエリ数、$T$ は区間の伸縮操作 (`add`, `erase`) 1回あたりの計算量。
多くの場合 $T=O(1)$ や $T=O(\log N)$ となり、全体で $O((N+Q)\sqrt{Q})$ や $O((N+Q)\sqrt{Q} \log N)$ となる。

## 参考
- [https://ei1333.hateblo.jp/entry/2017/09/11/211011](https://ei1333.hateblo.jp/entry/2017/09/11/211011)