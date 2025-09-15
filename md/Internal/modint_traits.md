---
title: ModInt型を判定する型特性 (Type Traits)
documentation_of: ../../src/Internal/modint_traits.hpp
---

## 概要

`is_modint_v`, `is_staticmodint_v` は、与えられた型 `T` が `ModInt` (固定法) もしくは `ModInt_Runtime` (実行時法) であるかをコンパイル時に判定するための型特性である。
主にライブラリ内部で、テンプレートの型制約 (SFINAEなど) に利用される。

`ModInt` は `s_b` を、`ModInt_Runtime` は `m_b` を継承しており、`s_b` は `m_b` を継承している。この継承関係を利用して判定を行っている。

## 型特性

| 名前                   | 概要                                     |
| ---------------------- | ---------------------------------------- |
| `is_modint_v<T>`       | `T` が `ModInt` または `ModInt_Runtime` なら `true` |
| `is_staticmodint_v<T>` | `T` が `ModInt` (固定法) なら `true` |

## 使用例

```cpp
#include "src/Math/ModInt.hpp"
#include "src/Math/ModInt_Runtime.hpp"
#include "src/Internal/modint_traits.hpp"

using static_mint = ModInt<998244353>;
using runtime_mint = ModInt_Runtime<0>;

// is_modint_v
static_assert(is_modint_v<static_mint>);
static_assert(is_modint_v<runtime_mint>);
static_assert(!is_modint_v<int>);

// is_staticmodint_v
static_assert(is_staticmodint_v<static_mint>);
static_assert(!is_staticmodint_v<runtime_mint>);
static_assert(!is_staticmodint_v<int>);
```
