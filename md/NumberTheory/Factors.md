---
title: 高速素因数分解など
documentation_of: ../../src/NumberTheory/Factors.hpp
---

## `Factors` クラス

`uint64_t` 型の整数 $n = p_1^{e_1}p_2^{e_2}\cdots p_k^{e_k}$ を素因数分解した結果を、`(素因数, 指数)` のペア `pair<u64, uint16_t>` の列として格納するクラス。
`constexpr` 対応。

| 名前 | 概要 | 計算量  |
| ---| --- | ---|
| `Factors(n)` | コンストラクタ。`uint64_t` 型の `n` を素因数分解する。 | $O(n^{1/4})$ |

## その他関数

対象の整数は `uint64_t` の範囲を想定する。

| 名前 | 概要 | 計算量  |
| ---| --- | ---|
| 1. `totient(n)` <br> 2. `totient(f)`  | オイラーのトーシェント関数 $\phi(n)$ を `uint64_t` 型で計算する。<br> 1. `n` を引数として与える。<br> 2. `Factors` クラスのオブジェクト `f` を直接引数で与える。<br> `constexpr` 対応。 | 1. $O(n^{1/4})$ <br> 2. $O(\log n)$ |
| 1. `enumerate_divisors<Uint>(n)`<br> 2. `enumerate_divisors<Uint>(f)` | `n` の約数列を `vector<Uint>` 型で返す。デフォルトは `uint64_t`。<br> 1. `n` を引数として与える。<br> 2. `Factors` クラスのオブジェクト `f` を直接引数で与える。<br> **注意**: 返される約数列はソートされていない。（おおむね小さい素因数から順に約数が構成される） | 1. $O(n^{1/4})$ <br> 2. $O(\text{約数の個数})$ |