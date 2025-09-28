---
title: 線形漸化的数列の第$k$項
documentation_of: ../../src/Math/bostan_mori.hpp
---

## 概要

Bostan-Mori法は、有理式 $P(x)/Q(x)$ の形式的冪級数展開における $k$ 次の係数 $\lbrack x^k \rbrack \frac{P(x)}{Q(x)}$ や、線形漸化式で表される数列の第 $k$ 項を高速に求めるためのアルゴリズムです。

---

## div_at

### 形式

```cpp
template <class K>
K div_at(std::vector<K> p, std::vector<K> q, uint64_t k);
```

### 説明

有理式 $\displaystyle \frac{P(x)}{Q(x)}$ を形式的冪級数として展開したときの $x^k$ の係数、すなわち $\lbrack x^k \rbrack \frac{P(x)}{Q(x)}$ の値を返します。

-   `p`: 分子多項式 $P(x)$ の係数を、次数の昇順で格納した `std::vector`。
-   `q`: 分母多項式 $Q(x)$ の係数を、次数の昇順で格納した `std::vector`。
-   `k`: 求めたい係数の次数。

### 制約

-   `q[0]` は乗法逆元を持つ必要があります。

### 計算量

$d = \max(\deg(P), \deg(Q))$ として、 $O(d^2 \log k)$。

---

## linear_recurrence

### 形式

```cpp
template <class K>
K linear_recurrence(std::vector<K> c, const std::vector<K> &a, uint64_t k);
```

### 説明

線形漸化式で定義される数列 $(b_n)$ の第 $k$ 項 $b_k$ を返します。
漸化式は以下のように定義されます。

$\displaystyle b_n = \begin{cases} a_n & n < d \ \sum_{i=0}^{d-1} c_i b_{n-1-i} & n \geq d \end{cases}$

これは $b_n = c_0 b_{n-1} + c_1 b_{n-2} + \dots + c_{d-1} b_{n-d}$ と同義です。

-   `c`: 漸化式の係数 $c_0, c_1, \dots, c_{d-1}$ を格納した `std::vector`。ここで $d = \text{c.size()}$ です。
-   `a`: 数列の初項 $b_0, b_1, \dots, b_{d-1}$ を格納した `std::vector`。
-   `k`: 求めたい項のインデックス。

### 制約

-   `c.size() <= a.size()`

### 計算量

$d = \text{c.size()}$ として、$O(d^2 \log k)$。

## 参考
- [線形漸化的数列のN項目の計算](https://qiita.com/ryuhe1/items/da5acbcce4ac1911f47a)
