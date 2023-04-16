---
title: Li-Chao-Tree
documentation_of: ../../src/DataStructure/LiChaoTree.hpp
---
浮動小数点も行けるはず \
最大値取得したいなら(-a,-b)を挿入して-最小値を取得
## 計算量
直線挿入: $\mathcal{O}(\log n)$　\
線分挿入: $\mathcal{O}((\log n)^2)$　\
最小値取得: $\mathcal{O}(\log n)$
## 問題例
[AtCoder Regular Contest 051 D - 長方形](https://atcoder.jp/contests/arc051/tasks/arc051_d) (double型)