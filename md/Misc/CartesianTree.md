---
title: Cartesian-Tree
documentation_of: ../../src/Misc/CartesianTree.hpp
---

![cartesian_tree](https://upload.wikimedia.org/wikipedia/commons/thumb/d/d5/Cartesian_tree.svg/864px-Cartesian_tree.svg.png)
[Wikipediaから引用](https://en.wikipedia.org/wiki/File:Cartesian_tree.svg)

## メンバ関数

| 名前                                   | 概要                                                                                                                                                                                                                                                                       | 計算量           |
| -------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------- |
| `CartesianTree(vector a, bool is_min)` | コンストラクタ.<br> 配列 $(a_n)$ をもとに構築. `is_min` が true なら 最小値, false なら 最大値 が親になるように構築. <br> 極大長方形のアルゴリズム.                                                                                                                        | $\mathcal{O}(N)$ |
| `range(int i)`                         | なるべく大きい半開区間 $[l_i, r_i)$ を返す. <br>ただし半開区間 $[l_i, r_i)$ は次の条件を満たす. <br> 1. $i \in [l_i,r_i)$<br> 2. `is_min` が true なら $a_i = \min \lbrace a_j : j \in [l_i,r_i) \rbrace$ , false なら  $a_i = \max \lbrace a_j : j \in [l_i,r_i) \rbrace$ |                  |
| `root()`                               | Cartesian-Treeの根を返す                                                                                                                                                                                                                                                   |                  |
| `parent(int i)`                        | Cartesian-Treeの $i$ の親を返す                                                                                                                                                                                                                                            |                  |
| `children(int i)`                      | Cartesian-Treeの $i$ の子を返す. 二分木なので`array<int,2>` で返す. いない場合 $-1$.                                                                                                                                                                                       |                  |


## 問題例
[AtCoder Beginner Contest 275 Ex - Monster](https://atcoder.jp/contests/abc275/tasks/abc275_h)