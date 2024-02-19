---
title: 最小全域有向木
documentation_of: ../../src/Graph/minimum_spanning_aborescense.hpp
---

|関数名|概要|計算量|
|---|---|---|
|`minimum_spanning_aborescense(g,w,root)`|最小全域有向木を求める．<br> 引数は [`Graph` クラス](Graph.hpp) と辺の重みを表す`vector`と求めたい木の根 root.<br>返り値は `pair` でラッピングした二つを返す．<br> 一つ目は MSA の重みの和．<br>二つ目は MSA に使う辺の番号を表す　`vector<int>`. |$O(E\log V)$|