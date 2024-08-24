---
title: Cartesian-Tree
documentation_of: ../../src/Misc/CartesianTree.hpp
---

<img src="https://github.com/hashiryo/Library/blob/master/img/cartesiantree.svg?raw=true" width="60%">

## メンバ関数

| 名前 | 概要| |
| --- |---| ---|
| `CartesianTree(Vec a, bool is_min)` | コンストラクタ.<br> 配列 $(a_n)$ をもとに構築. `is_min` が` true` なら 最小値, `false` なら 最大値 が親になるように構築. <br> 極大長方形のアルゴリズム.<br> 計算量は $O(N)$ ||
| `range(int i)`                         | 次の条件を満たす半開区間 $\lbrack l, r)$ を返す. <br> 1. $i \in \lbrace l,\dots,r-1\rbrace$<br> 2. $\min_{j \in \lbrace l,\dots,r-1\rbrace} \lbrace a_j  \rbrace = a_i$ <br> ( `is_min` が `true` の場合, `false` なら $\max$ ) <br> 3. 極大. つまり <br> $\min_{j \in \lbrace l-1,\dots,r-1\rbrace} \lbrace a_j  \rbrace \neq a_i$ かつ $\min_{j \in \lbrace l,\dots,r\rbrace} \lbrace a_j  \rbrace \neq a_i$ <br>( `is_min` が `true` の場合, `false` なら $\max$ ) |<img src="https://github.com/hashiryo/Library/blob/master/img/cartesiantree_range.svg?raw=true" width="100%">|
| `root()`                               | Cartesian-Treeの根を返す. ||
| `parent(int i)`                        | Cartesian-Treeの頂点 $i$ の親を返す. |<img src="https://github.com/hashiryo/Library/blob/master/img/cartesiantree_parent.svg?raw=true" width="100%">|
| `children(int i)`                      | Cartesian-Treeの頂点 $i$ の子を返す. <br> 二分木なので`array<int,2>` で返す. いない場合 $-1$.|<img src="https://github.com/hashiryo/Library/blob/master/img/cartesiantree_children.svg?raw=true" width="100%">|


## 問題例
[AtCoder Beginner Contest 275 Ex - Monster](https://atcoder.jp/contests/abc275/tasks/abc275_h)(木上をdfs)