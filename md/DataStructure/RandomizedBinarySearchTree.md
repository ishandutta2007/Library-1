---
title: Randomized-Binary-Search-Tree
documentation_of: ../../src/DataStructure/RandomizedBinarySearchTree.hpp
---
単位元は必要なし（遅延側も）\
各ノードが部分木のサイズを保持しているのでmapping関数では引数としてsizeを渡せる
## 計算量
$\mathcal{O}(\log N)$

## 問題例
[CODE FESTIVAL 2014 エキシビション B - カッコつけ](https://atcoder.jp/contests/code-festival-2014-exhibition/tasks/code_festival_exhibition_b) (insert, erase, fold)\
[SoundHound Programming Contest 2018 Masters Tournament 本戦 (Open) E - Hash Swapping](https://atcoder.jp/contests/soundhound2018-summer-final-open/tasks/soundhound2018_summer_final_e) (split3, fold)