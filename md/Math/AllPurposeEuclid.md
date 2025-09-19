---
title: 万能欧几里得 (All-Purpose Euclid)
documentation_of: ../../src/Math/AllPurposeEuclid.hpp
---

## 概要

`AllPurposeEuclid.hpp` は、2つのアフィン変換 $f_0(x) = x+1, f_1(x) = ax+b$ の合成列によって与えられる値の総和を高速に計算するアルゴリズム、通称「万能欧幾里得」の機能を提供します。
より具体的には、2種類の操作を表すモノイドの元 $S_U, S_R$ があり、パラメータ $(n, a, b, c)$ に対して、 $U^{b/c}$ と $rec(n, a, b \mod c, c, S_U, S_R)$ の積を計算します。ここで $rec$ は再帰的な処理を表します。
このフレームワークを用いることで、数え上げ問題などで現れる床関数を含む様々な形の総和を $O(\log n)$ ベースの計算量で求めることができます。

このライブラリでは、この汎用的なフレームワークを利用した具体的な応用として、以下の2つのクラスを提供します。

1.  `FloorSumTable`: $\sum_{i=0}^{n} i^{k_1} \lfloor \frac{ai+b}{c} \rfloor^{k_2}$ の形の和を計算します。
2.  `RingFloorSum`: $\sum_{i=0}^{n} A^i B^{\lfloor \frac{ai+b}{c} \rfloor}$ の形の和を計算します。

---

## `FloorSumTable`

### 機能

$\\sum_{i=0}^{n} i^{k_1} \lfloor \frac{ai+b}{c} \rfloor^{k_2}$ の形の和を、指定された次数の上限 $K_1, K_2$ まで、すべての組 $(k_1, k_2)$ ($0 \le k_1 \le K_1, 0 \le k_2 \le K_2$) についてまとめて計算します。

### テンプレートパラメータ

- `int_t`: 計算に使用する数値型（例: `long long`, `ModInt`）。
- `MXK1`: $i$ の次数の最大値。
- `MXK2`: $\lfloor \frac{ai+b}{c} \rfloor$ の次数の最大値。

### 使い方

#### `static void init()`
二項係数のテーブルを前計算します。`solve` を呼び出す前に、一度だけ呼び出す必要があります。

#### `static auto solve(u64 n, u64 a, u64 b, u64 c, int k1, int k2)`
- **引数**:
    - `n, a, b, c`: 和を計算する対象のパラメータ。 $i=0, \dots, n$ の $n+1$ 項について計算します。
    - `k1, k2`: 計算したい次数の上限。`k1 <= MXK1`, `k2 <= MXK2` を満たす必要があります。
- **戻り値**:
    - `std::array<std::array<int_t, MXK2 + 1>, MXK1 + 1>>` 型の2次元配列 `ret`。
    - `ret[i][j]` には $\sum_{k=0}^{n} k^i \lfloor \frac{ak+b}{c} \rfloor^j$ の値が格納されます。

### 計算量

$O((K_1 \cdot K_2)^2 \log(n+a+b+c))$

### コード例

```cpp
// problem: https://judge.yosupo.jp/problem/sum_of_floor_of_linear
// Sum of Floor of Linear: sum_{i=0}^{N-1} floor((A*i + B)/M) を求める

#include <iostream>
#include "src/Math/AllPurposeEuclid.hpp"

int main() {
    // long longで計算, iの次数は最大0, floor()の次数は最大1
    using FST = FloorSumTable<long long, 0, 1>;
    FST::init();

    long long N, M, A, B;
    std::cin >> N >> M >> A >> B;

    // i=0..N-1 の和を求めたいので、solveの第一引数には N-1 を渡す
    // k1=0, k2=1 を指定
    auto result_table = FST::solve(N - 1, A, B, M, 0, 1);

    // 求めたいのは sum(i^0 * floor^1) なので、table[0][1]
    long long result = result_table[0][1];
    std::cout << result << std::endl;

    return 0;
}
```

---

## `RingFloorSum`

### 機能

$\\sum_{i=0}^{n} A^i B^{\lfloor \frac{ai+b}{c} \rfloor}$ の形の和を計算します。　$A, B$ は環の元です。

### テンプレートパラメータ

- `R_t`: 計算に使用する環の型（例: `ModInt`）。
- `R_t (*ro)()`: `R_t` の加法単位元（$0$）を返す関数ポインタ。
- `R_t (*ri)()`: `R_t` の乗法単位元（$1$）を返す関数ポインタ。

### 使い方

#### `static R_t solve(u64 n, u64 a, u64 b, u64 c, const R_t &A, const R_t &B)`
- **引数**:
    - `n, a, b, c`: 和を計算する対象のパラメータ。 $i=0, \dots, n$ の $n+1$ 項について計算します。
    - `A, B`: `R_t` 型の基数。
- **戻り値**:
    - 計算結果の `R_t` 型の値。

### 計算量

$O(\log(n+a+b+c))$

### コード例

```cpp
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/Math/AllPurposeEuclid.hpp"

using Mint = ModInt<998244353>;

Mint R_zero() { return 0; }
Mint R_one() { return 1; }

int main() {
    using RFS = RingFloorSum<Mint, R_zero, R_one>;

    long long n = 100, a = 6, b = 5, c = 7;
    Mint A = 2, B = 3;

    // sum_{i=0}^{100} 2^i * 3^floor((6i+5)/7) mod 998244353 を計算
    Mint result = RFS::solve(n, a, b, c, A, B);
    std::cout << result << std::endl;

    return 0;
}
```

## Verify

- [Library Checker - Sum of Floor of Linear](https://judge.yosupo.jp/problem/sum_of_floor_of_linear) (`FloorSumTable`)
- [LOJ #138. 数列の和](https://loj.ac/p/138) (`FloorSumTable`)

## 参考
[https://www.luogu.com.cn/blog/ix-35/solution-p5170](https://www.luogu.com.cn/blog/ix-35/solution-p5170) \
[https://kewth.github.io/2020/07/19/%E4%B8%87%E8%83%BD%E6%AC%A7%E5%87%A0%E9%87%8C%E5%BE%97/](https://kewth.github.io/2020/07/19/%E4%B8%87%E8%83%BD%E6%AC%A7%E5%87%A0%E9%87%8C%E5%BE%97/) \
[https://www.cnblogs.com/p-b-p-b/p/13070809.html](https://www.cnblogs.com/p-b-p-b/p/13070809.html)