---
title: Functionalグラフ
documentation_of: ../../src/Graph/FunctionalGraph.hpp
---

## メンバ関数

| 名前 | 概要 | 計算量 |
| --- | --- | --- |
| `FunctionalGraph(to)` | コンストラクタ. <br> 各頂点の移動先を表す配列を渡す． <br> 引数は `vector<int>`. ||
| `jump(v,k)`          | 頂点 v から k ステップ 進んだ先の頂点を返す.  | $\mathcal{O}(\log N)$ |
| `path(v,k)`          | 頂点 v から k ステップ 進むときの頂点のパスを返す.<br> ただし パスは (サイクルに入るまで, サイクル, 余り) の3つに分解し, それぞれ何周するかの情報も持たせる. (1,3 番目の成分は 高々 1周) <br> 返り値は `array<pair<vector<int>,Int>,3>` | 出力に線形 |

## 問題例
[AtCoder Beginner Contest 030 D - へんてこ辞書](https://atcoder.jp/contests/abc030/tasks/abc030_d)