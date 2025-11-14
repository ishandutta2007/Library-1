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
[https://ja.wikipedia.org/wiki/疎行列#圧縮行格納](https://ja.wikipedia.org/wiki/%E7%96%8E%E8%A1%8C%E5%88%97#%E5%9C%A7%E7%B8%AE%E8%A1%8C%E6%A0%BC%E7%B4%8D) \
[https://nachiavivias.github.io/cp-library/cpp/array/csr-array.html](https://nachiavivias.github.io/cp-library/cpp/array/csr-array.html)