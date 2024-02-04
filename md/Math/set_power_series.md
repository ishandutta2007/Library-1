---
title: 集合冪級数
documentation_of: ../../src/Math/SetPowerSeries.hpp
---

集合 $S,T,U \in 2^{\lbrace0,1,\dots,n-1\rbrace}$ を2進数表記によって非負整数と同一視する.\
環 $K$ 上の集合冪級数 $f,g,h \in K\lbrack x_0,\dots,x_{n-1}\rbrack/(x_0^2,\dots,x_{n-1}^2)$ を 

$\displaystyle 
\begin{aligned}
f(X) = \sum_{S=0}^{2^n-1}f_SX^S, &&g(X) = \sum_{S=0}^{2^n-1}g_SX^S, &&h(X) = \sum_{S=0}^{2^n-1}h_SX^S
\end{aligned}
$

と表記する. ただし $X^S = \prod_{i\in S}x_i$ である.\
$K$は乗法の逆元は必要としない. (各演算は定義できる)

名前空間 `sps` の中に関数が置いてある.

一部，集合冪級数の演算ではないものが含まれているかも

## 関数

| 名前    | 概要  | 計算量    |
| --- | --- | --- |
| `subset_zeta(f)`   | $g_S = \sum_{T \subseteq S} f_T$ となる $g$ を返す.| $\mathcal{O}(n2^n)$ |
| `subset_mobius(f)`    | $f_S = \sum_{T \subseteq S} g_T$ となる $g$ を返す.| $\mathcal{O}(n2^n)$ |
| `supset_zeta(f)`   | $g_S = \sum_{S \subseteq T} f_T$ となる $g$ を返す.| $\mathcal{O}(n2^n)$ |
| `supset_mobius(f)`    | $f_S = \sum_{S \subseteq T} g_T$ となる $g$ を返す.| $\mathcal{O}(n2^n)$ |
| `or_convolve(f,g)`   | $h_S = \sum_{U\cup T = S} f_Ug_{T}$ となる $h$ を返す.  | $\mathcal{O}(n2^n)$    |
| `and_convolve(f,g)`   | $h_S = \sum_{U\cap T = S} f_Ug_{T}$ となる $h$ を返す.  | $\mathcal{O}(n2^n)$    |
| `convolve(f,g)`   | $h = fg$ つまり $h_S = \sum_{T \subseteq S} f_Tg_{S\backslash T}$ となる $h$ を返す.  | $\mathcal{O}(n^22^n)$    |
| `semi_relaxed_convolve(f, init, phi)` | $\displaystyle  h_S=\begin{cases}  \text{init} & S = \varnothing, \newline \phi_S\left(\sum_{T \subsetneq S} h_Tf_{S\backslash T}\right)& S \neq \varnothing \end{cases}$<br> となる $h$ を返す. <br> $\phi_S$ は `phi(int,T&)` という参照渡しの関数で与える. | $\mathcal{O}(n^22^n)$    |
| `self_relaxed_convolve<T>(phi, n)`    | $\displaystyle h_S=\begin{cases} 0 & S= \varnothing, \newline \phi_S\left(\frac{1}{2}\sum_{\varnothing \neq T \subsetneq S}  h_Th_{S\backslash T}\right) & S \neq \varnothing \end{cases}$<br> となる $h$ を返す. <br> $\phi_S$ は `phi(int,T&)` という参照渡しの関数で与える.　<br> | $\mathcal{O}(n^22^n)$    |
| `inv(f)`| $h = 1/f$ つまり <br> $\displaystyle \sum_{T \subseteq S} h_Tf_{S\backslash T} = \begin{cases}1 & S=\varnothing,\\ 0 & S\neq \varnothing\end{cases}$ <br>となる $h$  を返す. <br> $f_{\varnothing}=1$ でないと assert で落ちる.    | $\mathcal{O}(n^22^n)$    |
| `div(f,g)`| $h = f/g$ つまり $f_S = \sum_{T \subseteq S} h_Tg_{S\backslash T}$ となる $h$  を返す. <br> $f_{\varnothing}=1$ でないと assert で落ちる.    | $\mathcal{O}(n^22^n)$    |
| `exp(f)`| $\displaystyle  \exp(f) =  \sum_{i=0}^{\infty} \frac{f^i}{i!} $ を返す. <br> $f_{\varnothing}=0$ でないと assert で落ちる.    | $\mathcal{O}(n^22^n)$    |
| `log(f)`| $ \log f $ ( $\exp$ の逆関数 )を返す. <br> $f_{\varnothing}=1$ でないと assert で落ちる.    | $\mathcal{O}(n^22^n)$    |
| `egf_comp(F,f)`  | $\displaystyle  F(f) = \sum_{i=0}^{\infty} F_i\frac{f^i}{i!} $ を返す. <br> $f_{\varnothing}=0$ でないと assert で落ちる. | $\mathcal{O}(n^22^n)$    |
| `poly_comp(P,f)` | 多項式の合成 $\displaystyle  P(f) = \sum_{i=0}^{m-1}P_if^i $ を返す.   | $\mathcal{O}(n^22^n+nm)$ |
| `egf(f)`| $k=0,1,\dots,n$ について<br> $\displaystyle \left\lbrack X^{2^n-1}\right\rbrack\frac{f^k}{k!}$<br> を返す.  | $\mathcal{O}(n^22^n)$    |
| `egf(f,g)`   | $k=0,1,\dots,n$ について <br>$\displaystyle \left\lbrack X^{2^n-1}\right\rbrack\frac{f^k}{k!}g$<br> を返す. | $\mathcal{O}(n^22^n)$    |

## 参考
[https://github.com/EntropyIncreaser/ioi2021-homework/blob/master/thesis/main.tex](https://github.com/EntropyIncreaser/ioi2021-homework/blob/master/thesis/main.tex) \
[https://notes.sshwy.name/Math/Subset-Power-Series](https://notes.sshwy.name/Math/Subset-Power-Series) \
[https://maspypy.com/category/集合べき級数](https://maspypy.com/category/%e9%9b%86%e5%90%88%e3%81%b9%e3%81%8d%e7%b4%9a%e6%95%b0)
## 問題例
[Xmas Contest 2020 H - Hierarchical Phylogeny](https://atcoder.jp/contests/xmascon20/tasks/xmascon20_h) (self relax or 合成 1-√(1-2f)) \
[LibraOJ #2340. 「WC2018」州区划分](https://loj.ac/p/2340) (semi relax)
