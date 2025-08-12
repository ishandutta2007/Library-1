---
title: 一般グラフの最大マッチング
documentation_of: ../../src/Graph/general_matching.hpp
---
GabowのEdmonds' Algorithm

|関数名|概要|計算量|
|---|---|---|
|`generate_matching(Graph g, vector<int> partner = {})`| 無向グラフ g の最大マッチングの一例を返す.  <br> 引数は [`Graph`クラス](Graph.hpp). <br> 第二引数は推論補助(※)<br> 返り値は二つの要素を `pair` でラッピングしたものを返す．<br> 一つ目は最大マッチングに使用する辺の番号の集合を表す `vector<int>`．<br>　二つ目は各頂点のマッチング相手が記録 (noマッチなら -1) されている `vector<int>` ． |$O(VE \log V)$<br>はやい|

※ 各頂点の（最大マッチングとは限らない）マッチング相手が記録されている `vector<int>`　( 返り値の二つ目の形式と同じ ) を渡す. マッチングとして矛盾している場合の挙動は未定義．一度この関数を実行した後，（辺を一本追加あるいは削除などの）少しだけ変化させた場合の再計算を効率よくするためのもの．
## Verify
<details>
<summary>ここをクリックすると開きます</summary>

<input disabled="" type="checkbox"> [Chokudai SpeedRun 002 K - 種類数 β](https://atcoder.jp/contests/chokudai_S002/tasks/chokudai_S002_k) (二部マッチング, 頂点:2e5+4e5?, 辺:4e5?)\
<input disabled="" type="checkbox"> [JOI春合宿2016 マッチングコンテスト A - 一般最大マッチング](https://atcoder.jp/contests/joisc2016matching/tasks/joisc2016matching_a)\
<input disabled="" type="checkbox"> [Universal Online Judge #79. 一般图最大匹配](https://uoj.ac/problem/79)\
<input disabled="" type="checkbox"> [Universal Online Judge #171. 【WC2016】挑战NPC](https://uoj.ac/problem/171) (マッチング相手の調整が必要)\
<input disabled="" type="checkbox"> [The 2021 ICPC Asia Shanghai Regional Programming Contest L. Three,Three,Three](https://codeforces.com/gym/103446/problem/L)\
<input disabled="" type="checkbox"> [ICPC 2018-2019, NEERC B Bimatching](https://codeforces.com/contest/1089)

</details>