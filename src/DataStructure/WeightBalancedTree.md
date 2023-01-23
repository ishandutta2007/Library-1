---
title: 永続Weight-Balanced-Tree
documentation_of: ./WeightBalancedTree.hpp
---
永続平衡二分木 \
※これは永続化してます（逆に非永続にできるようにしてません）\
単位元は必要なし（遅延側も）\
各ノードが葉のサイズを保持しているのでmapping関数では引数としてsizeを渡せる 
## 計算量
$\mathcal{O}(\log N)$
## 問題例
[2012年 日本情報オリンピック春合宿 copypaste - コピー＆ペースト](https://atcoder.jp/contests/joisc2012/tasks/joisc2012_copypaste) (永続) \
[AtCoder Regular Contest 030 D - グラフではない](https://atcoder.jp/contests/arc030/tasks/arc030_4) (永続遅延伝搬)