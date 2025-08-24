---
title: Binary Indexed Tree
documentation_of: ../../src/DataStructure/BinaryIndexedTree.hpp
---

## 概要 (Description)

Binary Indexed Tree (BIT) は、フェニック木 (Fenwick Tree) とも呼ばれるデータ構造です。
以下の操作を対数時間で行うことができます。

- 配列の要素に値を加算する (一点加算)
- 配列の先頭から任意の位置までの要素の和を求める (区間和)

この実装は **0-indexed** です。

## API

| 関数 | 説明 | 計算量 |
|:---|:---|:---|
| `BinaryIndexedTree(n)` | サイズ `n` のBITを構築する。 | $O(N)$ |
| `BinaryIndexedTree(const vector<T>& y)` | `vector` `y` からBITを構築する。 | $O(N)$ |
| `add(int i, T a)` | `i` 番目の要素に `a` を加算する。 | $O(\log N)$ |
| `sum(int i)` | 半開区間 `[0, i)` の要素の和を返す。 | $O(\log N)$ |
| `sum(int l, int r)` | 半開区間 `[l, r)` の要素の和を返す。 | $O(\log N)$ |
| `operator[](int k)` | `k` 番目の要素の値を返す。 | $O(\log N)$ |
| `find(T k)` | `sum(i+1) > k` となる最小の `i` を返す (k番目の要素を探索)。BIT上の値が非負整数でなければ正しく動作しない。解が存在しない場合は `-1` を返す。 | $O(\log N)$ |

## 使用例 (Example)

```cpp
#include <iostream>
#include <vector>
#include "DataStructure/BinaryIndexedTree.hpp"

int main() {
    // サイズ 10 で初期化
    BinaryIndexedTree<int> bit(10);

    // a[3] に 5 を加算
    bit.add(3, 5);
    // a[5] に 2 を加算
    bit.add(5, 2);
    // a[5] に 3 を加算
    bit.add(5, 3);

    // [0, 6) の和 = a[0] + ... + a[5]
    // 5 + 2 + 3 = 10
    std::cout << "sum[0, 6): " << bit.sum(6) << std::endl;

    // [3, 6) の和 = a[3] + a[4] + a[5]
    // 5 + 0 + 5 = 10
    std::cout << "sum[3, 6): " << bit.sum(3, 6) << std::endl;

    return 0;
}
```

## 計算量 (Complexity)

- **構築**: $O(N)$
- **一点加算**: $O(\log N)$
- **区間和**: $O(\log N)$