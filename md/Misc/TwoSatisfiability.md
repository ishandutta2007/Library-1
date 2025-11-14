---
title: 2-SAT (2-Satisfiability)
documentation_of: ../../src/Misc/TwoSatisfiability.hpp
---

## 概要

2-Satisfiability (2-SAT) 問題を解くためのライブラリ。

各節が高々2つのリテラルの論理和 (OR) で構成される論理式 (2-CNF) が与えられたとき、その論理式を真 (TRUE) にするような変数の割り当てが存在するかどうかを判定し、存在するならばその一例を求める。

変数の数を $N$、節の数を $M$ とする。変数 $i \ (0 \le i < N)$ に対応するリテラルを $i$、その否定リテラルを `neg(i)` で表現する。

強連結成分分解 (SCC) を用いて、$O(N+M)$ で解くことができる。

## API

### `TwoSatisfiability(int n)`

変数の数を `n` として初期化する。変数は `0, 1, ..., n-1` でインデックスされる。

### `neg(int x)`

リテラル `x` の否定リテラルを表す整数を返す。
内部的には、リテラル `x` が `0, ..., n-1` のとき `x+n` を、`n, ..., 2n-1` のとき `x-n` を返す。

### `add_if(int u, int v)`

節 $(u \implies v)$ を追加する。これは $(\neg u \lor v)$ と同値。

### `add_or(int u, int v)`

節 $(u \lor v)$ を追加する。

### `add_nand(int u, int v)`

節 $\neg(u \land v)$ を追加する。これは $(\neg u \lor \neg v)$ と同値。

### `set_true(int u)`

変数 `u` が真であることを要求する節 $(u)$ を追加する。

### `set_false(int u)`

変数 `u` が偽であることを要求する節 $(\neg u)$ を追加する。

### `solve()`

論理式を充足する割り当てが存在するかを判定する。

- 存在する場合: `std::vector<bool>` を返す。返り値を `ret` とすると、`ret[i]` が変数 `i` の真偽値に対応する (`true` なら真、`false` なら偽)。
- 存在しない場合: 空の `std::vector` を返す。

## 使用例

例えば、論理式 $(x_0 \lor \neg x_1) \land (x_1 \lor x_2)$ を解く場合。

```cpp
#include <iostream>
#include <vector>
#include "src/Misc/TwoSatisfiability.hpp"

int main() {
    // 変数が3つ (x_0, x_1, x_2)
    TwoSatisfiability sat(3);

    // 節 (x_0 or !x_1) を追加
    sat.add_or(0, sat.neg(1));

    // 節 (x_1 or x_2) を追加
    sat.add_or(1, 2);

    auto result = sat.solve();

    if (result.empty()) {
        std::cout << "UNSATISFIABLE" << std::endl;
    } else {
        std::cout << "SATISFIABLE" << std::endl;
        for (int i = 0; i < 3; ++i) {
            std::cout << "x" << i << " = " << (result[i] ? "true" : "false") << std::endl;
        }
    }

    return 0;
}
```

```
SATISFIABLE
x0 = false
x1 = false
x2 = true
```
(注: 解は一意ではないため、出力は異なる場合があります)

