---
title: Splay-Tree
documentation_of: ./SplayTree.hpp
---
木の深さが10^5ぐらいになると再帰関数がスタックオーバーフロー起こすので注意 \
set_balance() でランダムにsplayするのでならすことができるはず \
単位元は必要なし（遅延側も）\
区間reverseができる。（半群の可換性を仮定していないので無駄にメモリと時間を使うけど）\
各ノードが部分木のサイズを保持しているのでmapping関数では引数としてsizeを渡せる
## 計算量
$\mathcal{O}(\log N)$