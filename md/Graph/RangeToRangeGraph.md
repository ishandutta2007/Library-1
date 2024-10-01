---
title: 区間に辺を張るテク
documentation_of: ../../src/Graph/RangeToRangeGraph.hpp
---

## `RangeToRangeGraph<cost_t=int>` クラス

|メンバ変数|概要|
|---|---|
|`graph`|区間に辺を張るテクによって構築された有向グラフ．<br> [`Graph` クラス](Graph.hpp)|
|`weight`|区間に辺を張るテクによって構築された有向グラフの辺の重み．<br> `vector<cost_t>`|


|メンバ関数|概要|計算量|
|---|---|---|
|`RangeToRangeGraph(n)`|コンストラクタ．区間の長さ $n$ を渡す．||
|`add(s,t,w=0)`|点 $s$ から 点 $t$ へ重み $w$ の有向辺を張る．|$O(1)$|
|`add_from_range(s_l,s_r,t,w=0)`|区間 $\lbrack s_l, s_r)$ から点 $t$ へ重み $w$ の有向辺を張る．|$O(\log n)$|
|`add_to_range(s,t_l,t_r,w=0)`|点 $s$ から区間 $\lbrack t_l, t_r)$ へ重み $w$ の有向辺を張る．|$O(\log n)$|
|`add_from_range_to_range(s_l,s_r,t_l,t_r,w=0)`|区間 $\lbrack s_l, s_r)$ から区間 $\lbrack t_l, t_r)$ へ重み $w$ の有向辺を張る．|$O(\log n)$|


## 問題例
[灘校文化祭コンテスト 2022 Day1 I - NPCA Kingdom](https://atcoder.jp/contests/nadafes2022_day1/tasks/nadafes2022_day1_i)
## 参考
[https://lorent-kyopro.hatenablog.com/entry/2020/07/24/170656](https://lorent-kyopro.hatenablog.com/entry/2020/07/24/170656)