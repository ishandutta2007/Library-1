---
title: Wavelet行列 (Wavelet Matrix)
documentation_of: ../../src/DataStructure/WaveletMatrix.hpp
---

## 概要

Wavelet行列は、数列 $v$ に対して以下のクエリを高速に処理するデータ構造です。

- 区間 $[l, r)$ に含まれる値で $k$ 番目に小さい/大きい値の取得
- 区間 $[l, r)$ に含まれる値で $[L, R)$ の範囲内のものの個数

コンストラクタに渡された数列 `v` は内部で座標圧縮されます。そのため、値そのものではなく、値の大小関係のみが重要な場合に有効です。

## 計算量

$N$ を数列の長さ、 $V$ を数列に含まれる値の種類数とする。

- 構築: $O(N \log V)$
- 各クエリ: $O(\log V)$

## コンストラクタ

`WaveletMatrix(const std::vector<T> &v)`

数列 `v` を元に Wavelet 行列を構築する。

## メソッド

### `kth_smallest(int l, int r, int k)`

区間 `[l, r)` において $k$ 番目 (0-indexed) に小さい値を返す。

- 制約: $0 \le l \le r \le N$, $0 \le k < r - l$

### `kth_largest(int l, int r, int k)`

区間 `[l, r)` において $k$ 番目 (0-indexed) に大きい値を返す。

- 制約: $0 \le l \le r \le N$, $0 \le k < r - l$

### `count(int l, int r, T ub)`

区間 `[l, r)` に含まれる `ub` 未満の要素数を返す。

- 制約: $0 \le l \le r \le N$

### `count(int l, int r, T lb, T ub)`

区間 `[l, r)` に含まれる `lb` 以上 `ub` 未満の要素数を返す。

- 制約: $0 \le l \le r \le N$

## 使用例

```cpp
#include <iostream>
#include <vector>
#include "src/DataStructure/WaveletMatrix.hpp"

int main() {
    std::vector<int> a = {5, 2, 8, 2, 7, 1, 6};
    WaveletMatrix<int> wm(a);

    // a[1, 5) = {2, 8, 2, 7} で 2番目に小さい値 (0-indexed)
    // ソートすると {2, 2, 7, 8} なので、答えは 7
    int val = wm.kth_smallest(1, 5, 2);
    std::cout << val << std::endl; // 7

    // a[0, 7) で 3 以上 7 未満の値の数
    // {5, 2, 8, 2, 7, 1, 6} のうち {5, 6} が該当
    int num = wm.count(0, 7, 3, 7);
    std::cout << num << std::endl; // 2
    return 0;
}
```