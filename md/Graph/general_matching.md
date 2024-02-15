---
title: 一般グラフの最大マッチング
documentation_of: ../../src/Graph/general_matching.hpp
---
GabowのEdmonds' Algorithm

|関数名|概要|
|---|---|
|`generate_matching(CSRArray<int> adj)`<br>`generate_matching(Graph g)`| 無向グラフ g の最大マッチングの一例を返す.  <br> 引数は頂点 to 頂点の隣接リスト([`CSRArray<int>`クラス](../Internal/ListRange.hpp)) もしくは [`Graph`クラス](Graph.hpp) で無向グラフを渡す. <br> 返り値は `vector<Edge>`.|

## 問題例
[Chokudai SpeedRun 002 K - 種類数 β](https://atcoder.jp/contests/chokudai_S002/tasks/chokudai_S002_k) (二部マッチング, 頂点:2e5+4e5, 辺:4e5) 