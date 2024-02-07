---
title: 乗法的関数テーブルや gcd 畳み込みなど
documentation_of: ../../src/NumberTheory/tables.hpp
---

## 関数

|名前|概要|計算量|
|---|---|---|
|`factorize(n)`|$n$ の素因数分解をした結果を `vector<pair<int,short>>` で返す.|$O(\log n)$|
|`multiplicative_table<T>(N,f)`| 乗法的関数 $f$ の値が入ったテーブルを返す. <br> 素冪の場合 $f(p^e)$ を表す `f(p,e)` を渡す. <br> サイズは $N+1$. |$O(N)$ <br> ただし $f(p^e)$ の計算量を $O(1)$ とした.|
|`mobius_table<T=int>(N)`|メビウス関数 $\mu(n)$ の値が入ったテーブルを返す.<br> サイズは $N+1$.| $O(N)$ |
|`totient_table<T=int>(N)`|オイラーのトーシェント関数 $\varphi(n)$ の値が入ったテーブルを返す.<br> サイズは $N+1$.| $O(N)$ |
|`divisor_zeta(a)`| 約数ゼータ変換 $\sum_{m\vert n}a(m)$ をおこなう. |$O(N \log \log N)$|
|`divisor_mobius(a)`| 約数メビウス変換 $\sum_{m\vert n}\mu(n/m)a(m)$ をおこなう. |$O(N \log \log N)$|
|`multiple_zeta(a)`| 倍数ゼータ変換 $\sum_{n\vert m}a(m)$ をおこなう. |$O(N \log \log N)$|
|`multiple_mobius(a)`| 倍数メビウス変換 $\sum_{n\vert m}\mu(m/n)a(m)$ をおこなう. |$O(N \log \log N)$|
|`gcd_convolve(a,b)`| $\displaystyle c(n)=\sum_{\gcd(i,j)=n}a(i)b(j)$ <br>となる $c$ を返す. |$O(N \log \log N)$|
|`lcm_convolve(a,b)`| $\displaystyle c(n)=\sum_{\mathrm{lcm}(i,j)=n}a(i)b(j)$ <br>となる $c$ を返す. |$O(N \log \log N)$|


## 参考
[https://37zigen.com/linear-sieve/](https://37zigen.com/linear-sieve/)
[https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5](https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5) \
[https://en.wikipedia.org/wiki/Dirichlet_convolution](https://en.wikipedia.org/wiki/Dirichlet_convolution) \
[https://maspypy.com/dirichlet-積と、数論関数の累積和](https://maspypy.com/dirichlet-%E7%A9%8D%E3%81%A8%E3%80%81%E6%95%B0%E8%AB%96%E9%96%A2%E6%95%B0%E3%81%AE%E7%B4%AF%E7%A9%8D%E5%92%8C)