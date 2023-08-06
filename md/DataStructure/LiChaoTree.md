---
title: Li-Chao-Tree
documentation_of: ../../src/DataStructure/LiChaoTree.hpp
---
浮動小数点も行けるはず \
templateの第二引数が true なら最大値取得, false なら最小値取得. デフォルトは false.

## メンバ関数
|名前|概要|計算量|
|---|---|---|
|`LiChaoTree(l,u,inf)`| コンストラクタ. <br>考える $x$ の範囲 $\lbrack l,r\rbrack$ と $y$ 方向の ($\infty$ とみなせるほどの) 十分大きい値を与える. <br> デフォルトは `l=-2e^9, r=2e^9, inf=numeric_limits<T>::max()/2` .||
|`insert_line(a,b)`|直線 $y=ax+b$ を挿入 | $\mathcal{O}(\log n)$　|
|`insert_segment(l,r,a,b)`| 線分 $y=ax+b\;(x\in\lbrack l,r \rbrack) $ を挿入 | $\mathcal{O}((\log n)^2)$|
|`query(x)` | $x$ における最小値(最大値) を返す |$\mathcal{O}(\log n)$ | 

## 問題例
[AtCoder Regular Contest 051 D - 長方形](https://atcoder.jp/contests/arc051/tasks/arc051_d) (doubleでac, 有理数クラスだとacできず...) \
[Yandex Contest Алгоритм 2022 K. Stepwise subsequence ](https://contest.yandex.com/contest/42710/problems/K) (有理数クラスでac, doubleだとwa)