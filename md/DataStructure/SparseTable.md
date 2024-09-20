---
title: Sparse-Table
documentation_of: ../../src/DataStructure/SparseTable.hpp
---

結合則と冪等則を満たす二項演算子を渡して区間のまとめあげを高速に行うデータ構造.\
冪等則を要求する代わりに disjoint-sparse-table より定数倍が良い.

## メンバ関数

|名前|概要|計算量|
|---|---|---|
|`SparseTable(v,f)`|コンストラクタ. <br> 配列 `v` と2変数引数の関数 `f` 渡す. |以下配列のサイズを $n$ とする. <br> $O(n\log n)$|
|`prod(l,r)`|半開区間$\lbrack l,r)$ について `f` でまとめあげた値を返す.|$O(1)$|