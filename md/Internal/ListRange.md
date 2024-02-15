---
title: CSR 表現を用いた二次元配列 他
documentation_of: ../../src/Internal/ListRange.hpp
---

## `ListRange<T>` クラス

|メンバ関数|概要|
|---|---|
|`size()`||
|`begin()`||
|`end()`||
|`operator[](i)`|参照値を返す.|

## `ConstListRange<T>` クラス

|メンバ関数|概要|
|---|---|
|`size()`||
|`begin()`||
|`end()`||
|`operator[](i)`|const 参照値を返す.|

## `CSRArray<T>` クラス

`vector<vector<T>>` の気持ちで使う．

|メンバ関数|概要|
|---|---|
|`size()`||
|`operator[](i)`| `ListRange` もしくは `ConstListRange` を返す.|


## 参考
[https://nachiavivias.github.io/cp-library/cpp/array/csr-array.html](https://nachiavivias.github.io/cp-library/cpp/array/csr-array.html)