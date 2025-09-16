---
title: 素数判定
documentation_of: ../../src/NumberTheory/is_prime.hpp
---

`unsigned long long` 型の整数が素数かどうかを判定する。

## 関数

| 名前          | 概要                                              | 計算量                  |
| ------------- | ------------------------------------------------- | ----------------------- |
| `is_prime(n)` | `n` が素数なら true, 素数でないなら false を返す。 | $O(\log n)$ |

## 説明

Miller-Rabin素数判定法に基づいています。
`unsigned long long` 型の整数 `n` ($0 \le n < 2^{64}$) に対して、決定的に正しい結果を返します。
`constexpr` なのでコンパイル時にも使用可能です。

## 使用例

```cpp
#include <iostream>
#include "NumberTheory/is_prime.hpp"

int main() {
    std::cout << is_prime(2) << std::endl; // 1
    std::cout << is_prime(7) << std::endl; // 1
    std::cout << is_prime(10) << std::endl; // 0
    std::cout << is_prime(998244353) << std::endl; // 1
    
    // constepxr
    constexpr bool p = is_prime(1000000007);
    static_assert(p);

    return 0;
}
```

