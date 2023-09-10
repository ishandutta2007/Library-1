---
title: 簡易版LARSCH
documentation_of: ../../src/Optimization/simplified_larsch_dp.hpp
---

## 関数

| 名前         | 概要                                                 | 計算量                         |
| ------------ | ---------------------------------------------------- | ------------------------------ |
| `simplified_larsch_dp(N,w)` | \\[\mathrm{dp}_ {i}=\begin{cases} 0 & i=0 \newline \min _{j\lt i}\lbrace\mathrm{dp}_j+w(i,j)\rbrace&i\gt 0\end{cases}\\] の形のDPを解く.<br>ただしコスト $w$ は Monge.<br> 返り値は$\mathrm{dp}_i$ ( $i=0,\dots,N$ の $N+1$ 成分 ) |           $\mathcal{O}(N\log N)$             |

## 参考
[https://noshi91.hatenablog.com/entry/2023/02/18/005856](https://noshi91.hatenablog.com/entry/2023/02/18/005856)
