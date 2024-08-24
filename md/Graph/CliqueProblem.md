---
title: クリーク問題
documentation_of: ../../src/Graph/CliqueProblem.hpp
---

内部でグラフを隣接行列で表現してる. 

## メンバ関数

| 名前                        | 概要                                        | 計算量                                          |
| --------------------------- | ------------------------------------------- | ----------------------------------------------- |
| `CliqueProblem(n)`          | コンストラクタ. グラフの頂点数 n を与える.  |                                                 |
| `add_edge(u,v)`             | 無向辺 (u,v) を加える.                      |                                                 |
| `get_max_clique()`          | 最大クリークの一つを返す.                   | $O(1.381^nn)$                         |
| `get_max_independent_set()` | 最大独立集合の一つを返す.                   | $O(1.381^nn)$                         |
| `get_min_vertex_cover()`    | 最小頂点被覆の一つを返す.                   | $O(1.381^nn)$                         |
| `enumerate_cliques(out)`    | すべてのクリークcに対してout(c) を実行する. | $O(n2^{\sqrt{2m}})$ <br> $m$ は辺の数 |


## 問題例
[CODE THANKS FESTIVAL 2017(Parallel) G - Mixture Drug](https://atcoder.jp/contests/code-thanks-festival-2017-open/tasks/code_thanks_festival_2017_g) (最大クリーク, 40頂点) \
[yukicoder No.382 シャイな人たち (2)](https://yukicoder.me/problems/no/382) (最大独立集合, 120頂点) \
[Maximum-Cup 2018 H - Maxmin Tour](https://atcoder.jp/contests/maximum-cup-2018/tasks/maximum_cup_2018_h) (二部グラフ マッチング, 600頂点)
