---
title: Knuth-Morris-Pratt
documentation_of: ../../src/String/KnuthMorrisPratt.hpp
---
## 計算量
テーブル構築 $\mathcal{O}(|S|)$ \
pattern_match(T) $T$の中から$S$と一致する部分を検索 $\mathcal{O}(|T|)$
## 問題例
[AtCoder Regular Contest 060 F - 最良表現](https://atcoder.jp/contests/arc060/tasks/arc060_d) (`period(i)`)