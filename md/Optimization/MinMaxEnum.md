---
title: 最大最小を指定するための列挙型
documentation_of: ../../src/Optimization/MinMaxEnum.hpp
---

## 概要
`MinMaxEnum` は、アルゴリズムやデータ構造が最小化問題と最大化問題のどちらを扱うかを指定するための列挙型である。

`Convex Hull Trick` や `Li Chao Tree` のように、最小値と最大値のクエリを両方扱えるデータ構造において、テンプレート引数として渡すことでその動作を切り替えるために使用される。

## 定義
`MinMaxEnum` は以下の2つの値を持つ。

- `MINIMIZE`: 最小化問題を指定する。内部的な値は `1`。
- `MAXIMIZE`: 最大化問題を指定する。内部的な値は `-1`。

```cpp
enum MinMaxEnum { MAXIMIZE = -1, MINIMIZE = 1 };
```

## 使用例

`LiChaoTree` で最小値を求めるクエリに応答するインスタンスを作成する例。

```cpp
#include "src/Optimization/LiChaoTree.hpp"
#include "src/Optimization/MinMaxEnum.hpp"

// 最小値を求めるLi Chao Treeを構築
auto cht = lct.make_tree<MINIMIZE>();

// 直線 a*x + b を追加
cht.insert(a, b);

// 位置 p での最小値をクエリ
auto min_val = cht.query(p).first;
```

`MAXIMIZE` を指定すれば、最大値を求めるインスタンスを作成できる。

```cpp
// 最大値を求めるLi Chao Treeを構築
auto cht = lct.make_tree<MAXIMIZE>();
```
