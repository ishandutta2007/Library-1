---
title: tupleやarrayに関するテンプレート 他
documentation_of: ../../src/Internal/tuple_traits.hpp
---

| 名前                 | 概要                                                                                             |
| ------------------- | ------------------------------------------------------------------------------------------------ |
|`tuple_like_v<T>`|`T` が tuple, array, pair なら true.|
|`array_like_v<T>`|`T` が array, pair<U,U>, tuple<U,...,U> なら true.|
|`to_tuple(t)`| `t` を tuple に変換したものを返す. tuple like じゃないなら void.|
|`forward_tuple(t)`|`t` を tuple に変換したもの (forwad) を返す. tuple like じゃないなら void.|
|`to_array(t)`|`t` を array に変換したものを返す. array like じゃないなら void.|
|`to_tuple_t<T>`|`T` を tuple に変換した型 を返す. tuple like じゃないなら void.|
|`to_array_t<T>`|`T` を array に変換した型 を返す. array like じゃないなら void.|
| `other_than_first_argument_type_t<T>`| `T=tuple<U,Args...>` から `tuple<Args...>` を作る |
|todo||