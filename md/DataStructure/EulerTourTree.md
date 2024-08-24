---
title: Euler-Tour-Tree
documentation_of: ../../src/DataStructure/EulerTourTree.hpp
---
単位元は必要あり（遅延側は必要なし）\
各ノードが部分木のサイズを保持しているのでmapping関数では引数としてsizeを渡せる \
頂点数は $2^{20}$ 個が限界
## 計算量
$O(\log N)$