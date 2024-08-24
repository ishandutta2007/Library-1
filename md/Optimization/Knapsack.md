---
title: 0-1 ナップサック問題 (半分全列挙)
documentation_of: ../../src/Optimization/Knapsack.hpp
---
計算量が n 倍 改善されてるやつ \
空間計算量的にも n は 50 ぐらいが限界か
## メンバ関数

| 名前         | 概要                                                 | 計算量                         |
| ------------ | ---------------------------------------------------- | ------------------------------ |
| `add(v,w)`   | 価値 v, 重さ w のアイテムを追加                      |                                |
| `build()`    | 下準備 (半分に分けて全列挙する)                      | $O(2^{\frac{n}{2}})$ |
| `solve(cap)` | 容量が cap 以下になるような価値の最大値を返す (尺取) | $O(2^{\frac{n}{2}})$ |

## 参考
[https://twitter.com/noshi91/status/1271857111903825920](https://twitter.com/noshi91/status/1271857111903825920) 
## 問題例
[東京海上日動 プログラミングコンテスト2020 D - Knapsack Queries on a tree](https://atcoder.jp/contests/tokiomarine2020/tasks/tokiomarine2020_d)