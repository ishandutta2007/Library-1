---
title: 多項式の評価点シフト
documentation_of: ../../src/Math/sample_points_shift.hpp
---

$n-1$ 次多項式 $f(x)$ の $x=0, 1, \dots, n-1$ における評価点の値 $y_i = f(i)$ が与えられたときに、$f(c), f(c+1), \dots$ の値を求める。

---

### 1点の評価 ($O(n)$)

`src/Math/sample_points_shift.hpp`

|名前|概要|計算量|
|---|---|---|
|`sample_points_shift(y, c)`| $f(0), \dots, f(n-1)$ の値のリスト `y` と点 `c` から $f(c)$ を求める。| $O(n)$|

#### 制約
- `FactorialPrecalculation` により階乗とその逆元が前計算されている必要がある。

#### Verify
- [AtCoder Regular Contest 033 D - 見たことのない多項式](https://atcoder.jp/contests/arc033/tasks/arc033_4)

---

### 複数点の評価 ($O(n \log n)$)

`src/FFT/sample_points_shift.hpp`

NTT(数論変換)を利用して、複数点の評価を高速に行う。

|名前|概要|計算量|
|---|---|---|
|`sample_points_shift(y, c, m)`| $f(0), \dots, f(n-1)$ の値のリスト `y` と点 `c`、個数 `m` から $f(c), f(c+1), \dots, f(c+m-1)$ のリストを求める。| $O(n \log n)$|

#### Verify
- [Yosupo Judge - Shift of Sampling Points of Polynomial](https://judge.yosupo.jp/problem/shift_of_sampling_points_of_polynomial)