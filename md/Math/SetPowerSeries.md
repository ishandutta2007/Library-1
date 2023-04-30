---
title: 集合冪級数
documentation_of: ../../src/Math/SetPowerSeries.hpp
---

実装上は $S,T \in 2^{\lbrace0,1,\dots,n-1\rbrace}$ を ２進数表記の非負整数で表現する. またここでも 不等式 $S<T$ は ２進数表記の非負整数の意味での大小関係を指すことにする.

## static メンバ関数


| 名前                                  | 概要                                                                                                                                                                                                                                                                                                                                    | 計算量                   |
| ------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------ |
| `subset_sum(f)`                       | $g(S) = \sum_{T \subseteq S} f(T)$ となる $g$ を返す.                                                                                                                                                                                                                                                                                   | $\mathcal{O}(n2^n)$      |
| `subset_sum_inv(f)`                   | $f(S) = \sum_{T \subseteq S} g(T)$ となる $g$ を返す.                                                                                                                                                                                                                                                                                   | $\mathcal{O}(n2^n)$      |
| `convolve(f,g)`                       | $h = fg$ つまり $h(S) = \sum_{T \subseteq S} f(T)g(S\backslash T)$ となる $h$ を返す.                                                                                                                                                                                                                                                   | $\mathcal{O}(n^22^n)$    |
| `semi_relaxed_convolve(g, init, phi)` | \\[ \begin{cases} f(\varnothing) = \text{init}& \\\\ f(S) = \phi_S\left(\sum_{T \subsetneq S} f(T)g(S\backslash T)\right) & S \neq \varnothing \end{cases}\\] となる $f$ を返す. <br> 実際は $\phi_S$ は `phi(int,T&)` で参照渡しの関数を与える.                                                                                        | $\mathcal{O}(n^22^n)$    |
| `self_relaxed_convolve<T>(n, phi)`    | \\[ \begin{cases} f(\varnothing) = 0 & \\\\ f(S) = \phi_S\left(\sum_{\varnothing \subsetneq T \subsetneq S, T < (S\backslash T)}  f(T)f(S\backslash T)\right) & S \neq \varnothing \end{cases}\\] となる $f$ を返す. <br> 実際は $\phi_S$ は `phi(int,T&)` で参照渡しの関数を与える.　<br>                                              | $\mathcal{O}(n^22^n)$    |
| `composite(f,F)`                      | \\[ F(f) = \sum_{i=0}^{\infty} \frac{F_i}{i!} f^i \\] を返す. <br> 逆元のない型でもOK <br> $f(\varnothing)=0$ でないと assert で死ぬ.                                                                                                                                                                                                   | $\mathcal{O}(n^22^n)$    |
| `exp(f)`                              | \\[ \exp(f) =  \sum_{i=0}^{\infty} \frac{1}{i!} f^i \\] を返す. <br> あるいは言い換えると $g(\varnothing)=1 $ かつ $\mathfrak{D}g = (\mathfrak{D}f) g$ つまり $ \vert S\vert g(S)=\sum_{T\subseteq S} \vert T \vert f(T)g(S\backslash T) $ を満たす $g$ を返す. <br> 逆元のない型でもOK <br> $f(\varnothing)=0$ でないと assert で死ぬ. | $\mathcal{O}(n^22^n)$    |
| `log(f)`                              | $ \log f $ を返す.  <br> あるいは言い換えると $g(\varnothing)=0$ かつ $\mathfrak{D}f = (\mathfrak{D}g) f$ つまり$ \vert S\vert f(S)=\sum_{T\subseteq S} \vert T \vert g(T)f(S\backslash T) $ を満たす $g$ を返す.<br> 逆元のない型でもOK <br> $f(\varnothing)=1$ でないと assert で死ぬ.                                                | $\mathcal{O}(n^22^n)$    |
| `pow(f,k)`                            | $f^k$ を返す. <br>逆元のない型でもOK                                                                                                                                                                                                                                                                                                    | $\mathcal{O}(n^22^n)$    |
| `polynomial_composite(f,g)`           | \\[ g(x) = \sum_{i=0}^{m-1}g_ix^i \\] <br> に対して $g(f)$ を返す.                                                                                                                                                                                                                                                                      | $\mathcal{O}(n^22^n+nm)$ |
| `egf(f)`                              | $k=0,1,\dots,n$ について \\[ \left\lbrack X^{\lbrack n \rbrack}\right\rbrack \frac{f^k}{k!}\\] を返す. <br>逆元のない型でもOK                                                                                                                                                                                                           | $\mathcal{O}(n^22^n)$    |

## 参考
[https://github.com/EntropyIncreaser/ioi2021-homework/blob/master/thesis/main.tex](https://github.com/EntropyIncreaser/ioi2021-homework/blob/master/thesis/main.tex) \
[https://notes.sshwy.name/Math/Subset-Power-Series](https://notes.sshwy.name/Math/Subset-Power-Series)
## 問題例
[Xmas Contest 2020 H - Hierarchical Phylogeny](https://atcoder.jp/contests/xmascon20/tasks/xmascon20_h) (self relax or 合成 1-√(1-2f)) \
[LibraOJ #2340. 「WC2018」州区划分](https://loj.ac/p/2340) (semi relax) \
[AtCoder Beginner Contest 253 Ex - We Love Forest](https://atcoder.jp/contests/abc253/tasks/abc253_h) (egf)
