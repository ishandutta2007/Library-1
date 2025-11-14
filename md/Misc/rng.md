---
title: 疑似乱数生成器 (RNG)
documentation_of: ../../src/Misc/rng.hpp
---

## 概要

Xorshift アルゴリズムに基づいた疑似乱数生成器です。
初回呼び出し時に `std::random_device` を用いてシードが自動的に初期化されます。

## 提供される関数

| 関数 | 説明 |
|---|---|
| `uint64_t rng()` | `[0, 2^64 - 1]` の範囲の `uint64_t` 型の整数乱数を生成して返します。 |
| `uint64_t rng(uint64_t lim)` | `[0, lim)` の範囲の `uint64_t` 型の整数乱数を生成して返します。 |
| `int64_t rng(int64_t l, int64_t r)` | `[l, r)` の範囲の `int64_t` 型の整数乱数を生成して返します。 |

## 使用例

```cpp
#include <iostream>
#include "src/Misc/rng.hpp"

int main() {
    // 0 から 2^64-1 までの乱数を取得
    uint64_t val1 = rng();
    std::cout << "Random uint64_t: " << val1 << std::endl;

    // 0 から 99 までの乱数を取得
    uint64_t val2 = rng(100);
    std::cout << "Random in [0, 100): " << val2 << std::endl;

    // 10 から 20 までの乱数を取得
    int64_t val3 = rng(10, 21);
    std::cout << "Random in [10, 21): " << val3 << std::endl;

    return 0;
}
```

## 計算量

全ての関数の計算量は $O(1)$ です。

## 注意事項

`rng(lim)` および `rng(l, r)` は、内部で剰余演算子 (`%`) を使用しているため、生成される乱数は厳密には一様分布ではありません。ただし、競技プログラミングの多くの文脈ではこの偏りは問題になりません。

## 参考

- [Xorshift - Wikipedia](https://ja.wikipedia.org/wiki/Xorshift)
