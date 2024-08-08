---
title: monotone minima
documentation_of: ../../src/Optimization/monotone_minima.hpp
---

## 関数

| 名前         | 概要                                                 | 計算量                         |
| ------------ | ---------------------------------------------------- | ------------------------------ |
| `monotone_minima(H,W,select)` | monotone な $H\times W$ 行列に対して実行.<br> 行列は陽には与えず`select`関数を渡す.<br> `select(i,j,k)`  は $(i,j)$ -成分より $(i,k)$ -成分の方が望ましいなら true を返すという関数. ただし，呼ばれるときは必ず $j\lt k$．<br> 返り値は各行 $i$ に対して最適解を達成する列方向 $j$ を返す.                |           $\mathcal{O}((H+W)\log H)$             |

## 参考
[https://topcoder-g-hatena-ne-jp.jag-icpc.org/spaghetti_source/20120923/1348327542.html](https://topcoder-g-hatena-ne-jp.jag-icpc.org/spaghetti_source/20120923/1348327542.html)
## 問題例
[COLOCON -Colopl programming contest 2018- Final C - スペースエクスプローラー高橋君](https://atcoder.jp/contests/colopl2018-final/tasks/colopl2018_final_c) \
[技術室奥プログラミングコンテスト#2 F - NPCの家 (NPC's House)](https://atcoder.jp/contests/tkppc2/tasks/tkppc2016_f) \
[COLOCON -Colopl programming contest 2018- D - すぬけそだて――トレーニング―― ](https://atcoder.jp/contests/colopl2018-qual/tasks/colopl2018_qual_d)