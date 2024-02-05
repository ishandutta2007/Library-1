---
title: 高速素因数分解など
documentation_of: ../../src/Math/Factors.hpp
---

## `Factors` クラス

$n = p_1^{e_1}p_2^{e_2}\cdots p_k^{e_k}$ を素因数分解した 列 $( (p_1,e_1), (p_2,e_2), \dots, (p_k,e_k) )$ を格納するクラス.\
`constexpr` できる.
| 名前 | 概要 | 計算量  |
| ---| --- | ---|
| `Factors(n)` |  コンストラクタ．  | $\mathcal{O} \left(n^{1/4}\right) $  |

## その他関数

| 名前 | 概要 | 計算量  |
| ---| --- | ---|
| 1. `totient(n)` <br> 2. `totient(f)`  | オイラーのトーシェント関数 $\phi(n)$ を計算する. <br> 1. $n$ を引数として与える. <br> 2. あらかじめ 素因数分解しておいて その列 $f = ( (p_1,e_1), (p_2,e_2), \dots, (p_k,e_k) )$ を直接引数で与える. <br> `constexpr` でよべる.| 1. $\mathcal{O} \left(n^{1/4}\right) $ <br> 2. $\mathcal{O} \left(\sum_{i=1}^k e_i\right) $      |
| 1. `enumerate_divisors(n)`<br> 2. `enumerate_divisors(f)` | $n$の約数列を返す. <br> 1. $n$ を引数として与える. <br> 2. あらかじめ 素因数分解しておいて その列 $f = ( (p_1,e_1), (p_2,e_2), \dots, (p_k,e_k) )$ を直接引数で与える. <br> 大きさでソートしていない. (素因数について辞書順みたいな順) | 1. $\mathcal{O} \left(n^{1/4}\right) $ <br> 2. $\mathcal{O} \left(\prod_{i=1}^k (e_i+1)\right) $ |

