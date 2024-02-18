---
title: 一般グラフの最大マッチング
documentation_of: ../../src/Graph/general_matching.hpp
---
GabowのEdmonds' Algorithm

|関数名|概要|計算量|
|---|---|---|
|`generate_matching(Graph g, vector<int> partner = {})`| 無向グラフ g の最大マッチングの一例を返す.  <br> 引数は [`Graph`クラス](Graph.hpp). <br> 第二引数は推論補助(※)<br> 返り値は `pair` でラッピングした二つを返す．<br> 一つ目は使用する辺のidを表す `vector<int>`．<br>　二つ目は各頂点のマッチング相手が記録されている `vector<int>` (いないなら -1 )． |$O(nm \log n)$<br> ただし頂点数，辺数をそれぞれ $n,m$ とした． <br>はやい|

※ 各頂点の（最大マッチングとは限らない）マッチング相手が記録されている `vector<int>`　( 返り値の二つ目の形式と同じ ) を渡す. マッチングとして矛盾している場合の挙動は未定義．一度この関数を実行した後，（辺を一本追加あるいは削除などの）少しだけ変化させた場合の再計算を効率よくするためのもの．
## 問題例
[Chokudai SpeedRun 002 K - 種類数 β](https://atcoder.jp/contests/chokudai_S002/tasks/chokudai_S002_k) (二部マッチング, 頂点:2e5+4e5, 辺:4e5) 