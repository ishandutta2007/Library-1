---
title: 二部グラフ
documentation_of: ../../src/Graph/BipartiteGraph.hpp
---

## `BipartiteGraph` クラス

[`Graph` クラス](Graph.hpp)　を継承．\
左右の頂点の数をそれぞれ $L$, $R$ として
- 左側の頂点を $0,1,\dots,L-1$
- 右側の頂点を $L, L+1,\dots,L+R-1$

とする. (何かと都合がいいため)

また辺の向きは常に左から右を向くようにする．（手動でする）

二部マッチングなどの関数の引数に使用する．ただの箱

|メンバ関数|概要|
|---|---|
|`left_size()`|左側の頂点の数 $L$ を返す|
|`right_size()`|右側の頂点の数 $R$ を返す|



## 関数

|関数|概要|計算量|
|---|---|---|
|`paint_two_colors(adj)`<br> `paint_two_colors(g)`|グラフを2色で塗り分ける．<br> 引数は頂点 → 頂点の隣接リスト([`CSRArray<int>`クラス](../Internal/ListRange.hpp)) もしくは [`Graph`クラス](Graph.hpp) で無向グラフを渡す. <br> 戻り値は頂点サイズの `vector<int>` で各頂点の色の割り当てを表す. <br> 二部グラフでない場合，空集合を返す．|$O(V+E)$|
|`graph_to_bipartite(g,color={})`|[`Graph` クラス](Graph.hpp)を頂点を2色に塗り分けてラベルを付け直して `BipartiteGraph` クラスにする. <br> 頂点のラベルを付け替えるだけのため，辺の順番は保たれている． <br> 引数は `Graph` クラスと色の割り当てが記録された `vector<int>`．<br> 第二引数は省略可能. <br> 戻り値は3つの要素を `tuple` でラッピングしたものを返す. <br> 一つ目は `BipartiteGraph`．<br> 二つ目は元のグラフの頂点番号から新しい番号を返す `vector<int>`．<br> 三つ目は新しいグラフの頂点番号から元のグラフの番号を返す `vector<int>`．<br> 二部グラフでない場合，空集合らを返す． |$O(V+E)$|
|`bipartite_matching<lexical=false>(bg)`|二部グラフの最大マッチングを実行する. <br> 引数は `BipartiteGraph`．戻り値は2つの要素を `pair` でラッピングしたものを返す．<br> 一つ目は最大マッチングに使われる辺の番号の集合を表す `vector<int>`．<br> 二つ目は各頂点のマッチング相手が記録 (noマッチなら -1) されている `vector<int>` ．<br> またテンプレート引数が `true` の場合，左側の頂点を順に並べた場合のマッチング相手が辞書順になるようにする．|1. $O(E\sqrt{V})$ <br> 2. テンプレート引数が `true` の場合 <br> $O(EV)$ |


二部グラフの最小頂点被覆は [Dulmage-Mendelsohn 分解](DulmageMendelsohn.hpp) で実装．

## 問題例
[CODE FESTIVAL 2017 qual B C - 3 Steps](https://atcoder.jp/contests/code-festival-2017-qualb/tasks/code_festival_2017_qualb_c) (二部グラフ塗り分け)\
[アルゴリズムと数学　演習問題集 047 - Bipartite Graph](https://atcoder.jp/contests/math-and-algorithm/tasks/math_and_algorithm_ao) (二部グラフ判定)\
[Chokudai SpeedRun 002 K - 種類数 β](https://atcoder.jp/contests/chokudai_S002/tasks/chokudai_S002_k) (二部マッチング, 頂点:2e5+4e5, 辺:4e5)