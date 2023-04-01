---
title: 離散対数
documentation_of: ./DiscreteLogarithm.hpp
---

半群 $E$ が 集合 $T$ に(右)作用するとする. \
入力 : $x\in E$, $s,t\in T$, $n\in \mathbb{N}$ \
出力 : $ \begin{cases} -1 & I = \empty, \\ \min I &\mathrm{otherwise}.\end{cases}$ ただし $I= \lbrace $ $ i \in \mathbb{N}:$ $0 \le i < n$ かつ $sx^i=t$  $\rbrace$ 

## メンバ関数

| 名前                                     | 概要                                                                                                                                                                                          | 計算量                                                                                                                  |
| ---------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| `DiscreteLogarithm(mapping,op,hash,lim)` | コンストラクタ.  <br> `mapping` : $ T \times E \rightarrow T$ ( 作用 ) <br> `op` : $E \times E \rightarrow E$ ( 半群 $E$ の二項演算 ) <br> `hash` : $T \rightarrow \mathbb{N}$ ( int に変換 ) |                                                                                                                         |
| `operator()(x,s,t,n)`                    | 離散対数を計算                                                                                                                                                                                | $\mathcal{O}(A\sqrt{n}+B\log{n})$ <br> ただし作用, 二項演算の 計算量 をそれぞれ $\mathcal{O}(A), \mathcal{O}(B)$ とした |

## 参考
[https://maspypy.com/モノイド作用に関する離散対数問題](https://maspypy.com/%E3%83%A2%E3%83%8E%E3%82%A4%E3%83%89%E4%BD%9C%E7%94%A8%E3%81%AB%E9%96%A2%E3%81%99%E3%82%8B%E9%9B%A2%E6%95%A3%E5%AF%BE%E6%95%B0%E5%95%8F%E9%A1%8C)