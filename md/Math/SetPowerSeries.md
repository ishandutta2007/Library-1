---
title: 集合冪級数
documentation_of: ../../src/Math/SetPowerSeries.hpp
---

集合 $S,T \in 2^{\lbrace0,1,\dots,n-1\rbrace}$ を2進数表記の非負整数で表現する.\
環 $K$ 上の集合冪級数 $f,g,h \in K^{2^n}$ を 
\\[
\begin{aligned}
f(X) = \sum_{S=0}^{2^n-1}f_SX^S, &&g(X) = \sum_{S=0}^{2^n-1}g_SX^S, &&h(X) = \sum_{S=0}^{2^n-1}h_SX^S
\end{aligned}
\\]
とする.\
$K$は乗法の逆元は必要としない. (各演算は定義できる)
## static メンバ関数


| 名前                                  | 概要                                                                                                                                                                                                                                                                      | 計算量                   |
| ------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------ |
| `subset_sum(f)`                       | $g_S = \sum_{T \subseteq S} f_T$ となる $g$ を返す.                                                                                                                                                                                                                       | $\mathcal{O}(n2^n)$      |
| `subset_sum_inv(f)`                   | $f_S = \sum_{T \subseteq S} g_T$ となる $g$ を返す.                                                                                                                                                                                                                       | $\mathcal{O}(n2^n)$      |
| `convolve(f,g)`                       | $h = fg$ つまり $h_S = \sum_{T \subseteq S} f_Tg_{S\backslash T}$ となる $h$ を返す.                                                                                                                                                                                      | $\mathcal{O}(n^22^n)$    |
| `semi_relaxed_convolve(g, init, phi)` | \\[ \begin{cases} f_{\varnothing} = \text{init}& \\\\ f_S = \phi_S\left(\sum_{T \subsetneq S} f_Tg_{S\backslash T}\right) & S \neq \varnothing \end{cases}\\] となる $f$ を返す. <br> 実際は $\phi_S$ は `phi(int,T&)` で参照渡しの関数を与える.                          | $\mathcal{O}(n^22^n)$    |
| `self_relaxed_convolve<T>(n, phi)`    | \\[\begin{cases} f_{\varnothing} = 0 & \\\\ f_S = \phi_S\left(\frac{1}{2}\sum_{\varnothing \neq T \subsetneq S}  f_Tf_{S\backslash T}\right) & S \neq \varnothing \end{cases}\\] となる $f$ を返す. <br> 実際は $\phi_S$ は `phi(int,T&)` で参照渡しの関数を与える.　<br> | $\mathcal{O}(n^22^n)$    |
| `composite(f,F)`                      | \\[ F(f) = \sum_{i=0}^{\infty} \frac{F_i}{i!} f^i \\] を返す. <br> $f_{\varnothing}=0$ でないと assert で落ちる.                                                                                                                                                            | $\mathcal{O}(n^22^n)$    |
| `exp(f)`                              | \\[ \exp(f) =  \sum_{i=0}^{\infty} \frac{1}{i!} f^i \\] を返す. <br> $f_{\varnothing}=0$ でないと assert で落ちる.                                                                                                                                                          | $\mathcal{O}(n^22^n)$    |
| `log(f)`                              | $ \log f $ を返す. <br> $f_{\varnothing}=1$ でないと assert で落ちる.                                                                                                                                                                                                       | $\mathcal{O}(n^22^n)$    |
| `pow(f,k)`                            | $f^k$ を返す.                                                                                                                                                                                                                                                             | $\mathcal{O}(n^22^n)$    |
| `polynomial_composite(f,P)`           | 多項式 \\[ P(x) = \sum_{i=0}^{m-1}P_ix^i \\] に対して $P(f)$ を返す.                                                                                                                                                                                                      | $\mathcal{O}(n^22^n+nm)$ |
| `egf(f)`                              | $k=0,1,\dots,n$ について \\[\left\lbrack X^{2^n-1}\right\rbrack\frac{1}{k!}f^k\\] を返す.                                                                                                                                                                                 | $\mathcal{O}(n^22^n)$    |
| `egf(f,g)`                            | $k=0,1,\dots,n$ について \\[\left\lbrack X^{2^n-1}\right\rbrack\frac{1}{k!}gf^k\\] を返す.                                                                                                                                                                                | $\mathcal{O}(n^22^n)$    |

## 参考
[https://github.com/EntropyIncreaser/ioi2021-homework/blob/master/thesis/main.tex](https://github.com/EntropyIncreaser/ioi2021-homework/blob/master/thesis/main.tex) \
[https://notes.sshwy.name/Math/Subset-Power-Series](https://notes.sshwy.name/Math/Subset-Power-Series) \
[https://maspypy.com/category/集合べき級数](https://maspypy.com/category/%e9%9b%86%e5%90%88%e3%81%b9%e3%81%8d%e7%b4%9a%e6%95%b0)
## 問題例
[Xmas Contest 2020 H - Hierarchical Phylogeny](https://atcoder.jp/contests/xmascon20/tasks/xmascon20_h) (self relax or 合成 1-√(1-2f)) \
[Xmas Contest 2022 F - Fast as Fast as Ryser](https://atcoder.jp/contests/xmascon22/tasks/xmascon22_f) (exp, egf)\
[AtCoder Beginner Contest 253 Ex - We Love Forest](https://atcoder.jp/contests/abc253/tasks/abc253_h) (egf)\
[AtCoder Beginner Contest 236 Ex - Distinct Multiples](https://atcoder.jp/contests/abc236/tasks/abc236_h) (exp) \
[LibraOJ #2340. 「WC2018」州区划分](https://loj.ac/p/2340) (semi relax)
