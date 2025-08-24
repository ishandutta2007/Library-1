---
title: ディリクレ級数
documentation_of: ../../src/NumberTheory/DirichletSeries.hpp
---

$\newcommand{\floor}[1]{\left\lfloor{#1}\right\rfloor}$

数論的関数の累積和 $\sum_{i=1}^N f(i)$ を高速に求めるためのライブラリです。
ディリクレ積で表現できる数論的関数 $f(n) = (g*h)(n) = \sum_{d|n} g(d)h(n/d)$ の累積和を、$g, h$ の累積和が高速に求まる場合に高速に計算できます。
## `DirichletSeries<T>` クラス
ある数論的関数 $f$ について、$F(k) = \sum_{n=1}^k f(n)$ の値を $\lfloor N/m \rfloor$ の形で表せる全ての $k$ について保持するデータ構造です。

内部では、$f(n)$ の値を $n \le K$ まで、また $F(k)$ の値を $k \le K$ と $k=\lfloor N/m \rfloor$ ($m \le L$) について保持します。ここで、$K, L$ は $KL \ge N$ を満たすパラメータで、デフォルトでは $K \approx N^{2/3}, L \approx N^{1/3}$ となるように設定されます。これにより、多くの操作が $O(N^{2/3})$ 程度の計算量で実現されます。

ディリクレ級数の単位元 $\varepsilon(n)$ は以下のように定義されます。
$\displaystyle 
\varepsilon(n) \equiv \lbrack n=1 \rbrack 
\equiv 
\begin{cases}
1 & (n=1) \\
0 & (n > 1)
\end{cases}$

### メンバ変数

|メンバ変数|概要|
|---|---|
|`N`|累積和を計算する上限 $N$ です。|
|`K`, `L`|内部で利用するテーブルサイズです。$KL \ge N$ を満たします。|
|`x`| $f(1), \dots, f(K)$ の値を保持する配列です。|
|`X`| $F(1), \dots, F(K)$ および $F(\lfloor N/1 \rfloor), \dots, F(\lfloor N/L \rfloor)$ の値を保持する配列です。|

### 演算子オーバーロード

$f, g$ に対応する `DirichletSeries` オブジェクトから $h$ に対応するオブジェクトを計算します。

|演算子|概要|計算量|
|---|---|---|
|`-f`|$h(n)=-f(n)$|$O(K+L)$|
|`f+g`|$h(n)=f(n)+g(n)$|$O(K+L)$|
|`f+a` |$h(n)=f(n)+a\cdot\varepsilon(n)$|$O(1)$|
|`f-g`|$h(n)=f(n)-g(n)$|$O(K+L)$|
|`f-a`|$h(n)=f(n)-a\cdot\varepsilon(n)$|$O(1)$|
|`f*g` (ディリクレ積)|$\displaystyle h(n)=\sum_{d|n}f(d)g(n/d)$|$O(K\log K+\sqrt{NL})$|
|`f*a`|$h(n)= f(n)\cdot a$|$O(K+L)$|
|`f/g` (ディリクレ逆数)|$\displaystyle f(n) = \sum_{d|n}h(d)g(n/d)$ を満たす $h$|$O(K\log K+\sqrt{NL})$|
|`f/a`|$h(n)=f(n) /a$|$O(K+L)$|
|`a/f`|$\displaystyle a\cdot \varepsilon(n) = \sum_{d|n}h(d)f(n/d)$ を満たす $h$|$O(K\log K+\sqrt{NL})$|

### メンバ関数

|メンバ関数|概要|計算量|
|---|---|---|
|`DirichletSeries(N, unit=false)`|コンストラクタ。`unit`が`false`なら零元 ($f(n)=0$)、`true`なら単位元 ($f(n)=\varepsilon(n)$) で初期化します。| $O(K+L)$ |
|`DirichletSeries(N, sum)`|コンストラクタ。累積和を返す関数 `sum` を用いて初期化します。| $O(K+L)$ |
|`sum(k)`| $F(k) = \sum_{n=1}^k f(n)$ を返します。$k$ は $\lfloor N/m \rfloor$ の形で表せる必要があります。|$O(1)$|
|`sum()`|$\displaystyle F(N)=\sum_{n=1}^N f(n)$ を返します。|$O(1)$|
|`square()`|$f*f$ を計算します。`f*f`より高速です。|$O(K\log K+\sqrt{NL})$|
|`pow(M)`|ディリクレ積の $M$ 乗を計算します。|$O((K\log K+\sqrt{NL})\log M)$|



## 具体的な数論的関数の `DirichletSeries<T>`

一般的な数論的関数に対応する `DirichletSeries` オブジェクトを生成するヘルパー関数です。

|関数|数論的関数 $f(n)$|計算量|
|---|---|---|
|`get_1(N)`|$f(n) = 1$ (定数関数)| $O(K+L)$|
|`get_mu(N)`|$f(n) = \mu(n)$ (メビウス関数)| $O(K\log K+\sqrt{NL})$|
|`get_Id(N)`|$f(n) = n$ (恒等写像)| $O(K+L)$|
|`get_Id2(N)`|$f(n) = n^2$| $O(K+L)$|
|`get_d(N)`|$f(n) = d(n)$ (約数関数)| $O(K\log K+\sqrt{NL})$|
|`get_sigma(N)`|$f(n) = \sigma(n)$ (約数和関数)| $O(K\log K+\sqrt{NL})$|
|`get_phi(N)`|$f(n) = \phi(n)$ (オイラーのトーシェント関数)| $O(K\log K+\sqrt{NL})$|
|`get_1sq(N)`|$f(n) = [n \text{ is a perfect square}]$| $O(K+L)$|
|`get_lambda(N)`|$f(n) = \lambda(n)$ (リウヴィル関数)| $O(K\log K+\sqrt{NL})$|
|`get_absmu(N)`|$f(n) = \lvert\mu(n)\rvert$ (平方因子を持たないなら1)| $O(K\log K+\sqrt{NL})$|


## 参考
[https://maspypy.com/dirichlet-積と、数論関数の累積和](https://maspypy.com/dirichlet-%E7%A9%8D%E3%81%A8%E3%80%81%E6%95%B0%E8%AB%96%E9%96%A2%E6%95%B0%E3%81%AE%E7%B4%AF%E7%A9%8D%E5%92%8C)

## 使用例

- [Sum of Totient Function (yosupo)](../../test/yosupo/math/sum_of_totient_function.test.cpp)
  - $\sum_{i=1}^N \phi(i)$ を求めます。
- [Sum of Divisors (yosupo)](../../test/yosupo/math/sum_of_powers_of_divisors.test.cpp)
  - $\sum_{i=1}^N \sigma(i)$ を求めます。
- [Xmas Contest 2019 D - Sum of (-1)^f(n)](https://atcoder.jp/contests/xmascon19/tasks/xmascon19_d)
  - リウヴィル関数の累積和を求めます。