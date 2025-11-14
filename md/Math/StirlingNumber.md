---
title: スターリング数 (小さい素数 mod)
documentation_of: ../../src/Math/StirlingNumber.hpp
---

## 概要
$n$ が大きい場合に、小さい素数 $p$ を法とした第一種スターリング数 $s(n, k) \pmod p$ および第二種スターリング数 $S(n, k) \pmod p$ を高速に計算する。

Lucasの定理と同様の性質を利用して、$O(p^2)$ の前処理ののち、クエリあたり $O(\log_p n)$ で計算する。

## 制約
- `p` は素数であること
- `p < 32768`

## 使用方法
`StirlingNumber sn(p, first, second)`
- `p`: 素数の法
- `first`: `true` の場合、第一種スターリング数のための前処理を行う (デフォルト: `true`)
- `second`: `true` の場合、第二種スターリング数のための前処理を行う (デフォルト: `true`)

`sn.S1(n, k)`
- 第一種スターリング数 $s(n, k) \pmod p$ を返す。

`sn.S2(n, k)`
- 第二種スターリング数 $S(n, k) \pmod p$ を返す。

`sn.nCk(n, k)`
- Lucasの定理を利用して $nCk \pmod p$ を返す。

## 計算量
- 前処理: $O(p^2)$
- クエリ: $O(\log_p n)$

## コード
```cpp
#include <iostream>
#include "src/Math/StirlingNumber.hpp"

int main() {
    long long n = 1000000000000000000;
    long long k = 500000000000000000;
    int p = 101;

    // 第二種スターリング数のみ計算する場合
    StirlingNumber sn(p, false, true);

    std::cout << sn.S2(n, k) << std::endl;
}
```

## 参考
- [https://maspypy.com/stirling-数を-p-で割った余りの計算](https://maspypy.com/stirling-%e6%95%b0%e3%82%92-p-%e3%81%a7%e5%89%b2%e3%81%a3%e3%81%9f%e4%bd%99%e3%82%8a%e3%81%ae%e8%a8%88%e7%ae%97)
