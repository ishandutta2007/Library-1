---
title: 黄金分割探索 (Golden-section search)
documentation_of: ../../src/Optimization/golden_search.hpp
---

## 概要

実数上の単峰関数 (unimodal function) の最小値または最大値を求めるアルゴリズム。探索範囲を黄金比を用いて狭めていくことで、効率的に最適解を探索する。

対象とする関数の引数の型は `long double`。引数の型が整数な単峰関数を対象にする場合は[フィボナッチ探索](src/Optimization/fibonacci_search.hpp) を使う。

### 計算量

関数 `f(x)` の評価に $O(A)$ の計算量がかかるとしたとき、全体の計算量は $O(A \cdot \text{iter})$ となる。
ここで `iter` は反復回数。

## インクルード

```cpp
#include "src/Optimization/golden_search.hpp"
```

## 関数

| 名前 | 概要 |
|---|---|
| `golden_search<obj>(f, l, r, iter=100)` | 区間 $\lbrack l, r\rbrack$ 上で単峰となる関数 $f(x)$ の最適値とその最適解を返す。<br> **テンプレート引数 `obj`**: `MinMaxEnum::MINIMIZE` (最小化) または `MinMaxEnum::MAXIMIZE` (最大化) を指定する。<br> **引数**: <br> - `f`: 対象の関数オブジェクト (ラムダ式など)。`long double` を受け取り、評価値を返す。<br> - `l`, `r`: 探索範囲の下限と上限 (`long double`)。<br> - `iter`: 反復回数 (`int`)。デフォルトは100。精度に影響する。<br> **返り値**: `{最適解, 最適値}` の `std::pair<long double, ...>`。 |

## 使用例

### 最小化

二次関数 $f(x) = (x-2)^2$ の最小値を求める。

```cpp
#include <iostream>
#include <iomanip>
#include "src/Optimization/golden_search.hpp"

int main() {
    auto f = [](long double x) {
        return (x - 2.0) * (x - 2.0);
    };

    // 探索範囲 [-10.0, 10.0] で f(x) を最小化する
    auto result = golden_search<MINIMIZE>(f, -10.0, 10.0);

    std::cout << std::fixed << std::setprecision(10);
    std::cout << "x_min = " << result.first << std::endl;
    std::cout << "f(x_min) = " << result.second << std::endl;

    return 0;
}
```

#### 出力

```
x_min = 2.0000000000
f(x_min) = 0.0000000000
```