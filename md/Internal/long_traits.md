---
title: より大きい型に変換する型特性 (long_traits)
documentation_of: ../../src/Internal/long_traits.hpp
---

## 概要

`make_long_t<T>` は、与えられた型 `T` を、より表現範囲の広い型に変換するための型特性である。

例えば、幾何ライブラリにおいて `int` 型の座標を持つ2点の外積を計算する場合、結果が `int` の最大値を超える可能性がある。このようなオーバーフローを防ぐため、一時的に `long long` 型で計算を行うといった用途で用いる。

## 型の対応

| 元の型 (`T`)        | 変換後の型 (`make_long_t<T>`) |
| ------------------- | ----------------------------- |
| `char`              | `short`                       |
| `unsigned char`     | `unsigned short`              |
| `short`             | `int`                         |
| `unsigned short`    | `unsigned`                    |
| `int`               | `long long`                   |
| `unsigned`          | `unsigned long long`          |
| `long long`         | `__int128_t`                  |
| `unsigned long long`| `__uint128_t`                 |
| `float`             | `double`                      |
| `double`            | `long double`                 |

## 使用例

`int` 型の座標を持つ2つのベクトル `p`, `q` の外積 `cross(p, q)` を計算する。

```cpp
#include "src/Geometry/Point.hpp"

// K=int の場合、make_long_t<K> は long long になる
template <class K>
make_long_t<K> cross(const Point<K> &p, const Point<K> &q) {
    // p.x, p.y, q.x, q.y が int でも、long long にキャストされてから計算される
    return make_long_t<K>(p.x) * q.y - make_long_t<K>(p.y) * q.x;
}
```