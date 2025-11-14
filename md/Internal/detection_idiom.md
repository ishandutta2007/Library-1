---
title: detection idiom
documentation_of: ../../src/Internal/detection_idiom.hpp
---

## 概要

C++ の SFINAE (Substitution Failure Is Not An Error) を利用して、テンプレートパラメータがある特定の性質（メンバ型、メンバ関数など）を持つかどうかをコンパイル時に判定するためのマクロを提供する。

これにより、テンプレートクラスや関数において、与えられた型に応じて `if constexpr` などで処理を分岐させ、機能の有効化・無効化を自動的に切り替えることができる。

## マクロ

### `_DETECT_BOOL(name, ...)`

指定した式 `...` が有効な型や式であるかを判定し、結果を `bool` 型の定数として返すメタ関数 `name` とその値 `name_v` を定義する。

- `name<T>::value` および `name_v<T>` は、`T` に対して `...` が有効な場合は `true`、そうでない場合は `false` となる。

#### 使用例

`SegmentTree` では、テンプレートパラメータ `M` がモノイドの性質（型 `T`、二項演算 `op`、単位元 `ti`）を持つかを判定するために利用されている。

```cpp
_DETECT_BOOL(monoid, typename T::T, decltype(&T::op), decltype(&T::ti));

// 上記マクロにより、以下のようなメタ関数が定義される
template <class T>
static constexpr bool monoid_v = monoid<T>::value;

// monoid_v<M> を使って処理を分岐
if constexpr (monoid_v<M>) {
  // M がモノイドの性質を持つ場合の処理
}
```

### `_DETECT_TYPE(name, type1, type2, ...)`

指定した式 `...` が有効な型や式であるかに応じて、異なる型を定義するメタ関数 `name` を定義する。

- `name<T>::type` は、`T` に対して `...` が有効な場合は `type1`、そうでない場合は `type2` となる。

#### 使用例

`SegmentTree` では、テンプレートパラメータ `M` が作用素の型 `E` を持つかを判定し、持たない場合は `std::nullptr_t` で代用するために利用されている。

```cpp
_DETECT_TYPE(nullptr_or_E, typename T::E, std::nullptr_t, typename T::E);

// 上記マクロにより、以下のようなメタ関数が定義される
template <class T>
using nullptr_or_E_t = typename nullptr_or_E<T>::type;

// M が E を持てば E、持たなければ std::nullptr_t となる
using E = nullptr_or_E_t<M>;
```