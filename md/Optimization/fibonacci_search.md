---
title: フィボナッチ探索
documentation_of: ../../src/Optimization/fibonacci_search.hpp
---

## 概要

整数区間 `[l, r]` 上で単峰性を示す関数の最小値または最大値を求める。

引数の型が整数な単峰関数を対象とする。浮動小数点数を対象とする場合は[黄金分割探索](golden_search.md) を使う。

## 使い方

```cpp
#include "src/Optimization/fibonacci_search.hpp"
#include "src/Optimization/MinMaxEnum.hpp"

// pair{最適解, 最適値} fibonacci_search<obj>(f, l, r);
```

- `obj`: `MINIMIZE` (最小化) または `MAXIMIZE` (最大化) を指定する。
- `f`: `int64_t` を引数とし、比較可能な値を返す単峰関数オブジェクト。
- `l`, `r`: 探索範囲の閉区間 `[l, r]` (`int64_t`)。
- **返り値**: `{最適解, 最適値}` の `std::pair`。

### 使用例

$f(x) = (x-5)^2$ の最小値を求める。

```cpp
#include <iostream>
#include "src/Optimization/fibonacci_search.hpp"
#include "src/Optimization/MinMaxEnum.hpp"

int main() {
    auto f = [](long long x) {
        return (x - 5) * (x - 5);
    };
    // 探索範囲 [0, 10] で最小値を求める
    auto result = fibonacci_search<MINIMIZE>(f, 0, 10);
    // 最適解: 5, 最適値: 0
    std::cout << "x = " << result.first << ", min_val = " << result.second << std::endl;
    return 0;
}
```

## 制約

- `l <= r`
- 関数 `f` は探索区間 `[l, r]` で単峰性を持つ必要がある。

## 計算量

$N = r - l + 1$ とし、関数 `f` の評価に $O(A)$ の計算量がかかるとする。
- $O(A \log N)$

## Verify

- [京セラプログラミングコンテスト2023(AtCoder Beginner Contest 305) Ex - Shojin](https://atcoder.jp/contests/abc305/tasks/abc305_h) (Alien DP)
- [技術室奥プログラミングコンテスト#2 F - NPCの家 (NPC's House)](https://atcoder.jp/contests/tkppc2/tasks/tkppc2016_f) (Alien DP)

## 参考
- [フィボナッチ探索と黄金分割探索 - Qiita](https://qiita.com/tanaka-a/items/f380257328da421c6584)