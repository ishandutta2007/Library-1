---
title: Cartesian-Tree
documentation_of: ./CartesianTree.hpp
---
極大長方形のアルゴリズム
## メンバ関数
コンストラクタ `CartesianTree(vector a, bool is_min)` : 配列 $(a_n)$ をもとに構築. `is_min` が True なら 最小値, False なら 最大値 が親になるように構築 \
`range(int i)` : $a_i \in [l_i,r_i)$, $\forall j \in [l_i,r_i)\  a_j \ge (\text{or} \le\;) \;a_i$ を満たすもののうち $r_i-l_i$ が最大になるような 半開区間$[l_i, r_i)$ を返す \
`root()`: Cartesian-Treeの根を返す \
`parent(int i)`: Cartesian-Treeの$i$の親を返す \
`children(int i)`: Cartesian-Treeの$i$の子を返す. 二分木なので`array<int,2>` で返す. いない場合 $-1$.

## 計算量
構築 $\mathcal{O}(N)$

## 問題例
[AtCoder Beginner Contest 275 Ex - Monster](https://atcoder.jp/contests/abc275/tasks/abc275_h)