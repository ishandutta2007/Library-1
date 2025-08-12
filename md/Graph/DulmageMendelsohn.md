---
title: Dulmage-Mendelsohn 分解
documentation_of: ../../src/Graph/DulmageMendelsohn.hpp
---

## `DulmageMendelsohn` クラス

二部グラフを Dulmage-Mendelsohn 分解によって
K+1 個 の集合(0~K) に分割する．
- 左側の頂点： 0~K-1番目のいずれかの集合に属する $\iff$ マッチングに必ず使う
- 右側の頂点： 1~K番目のいずれかの集合に属する $\iff$ マッチングに必ず使う
- 左側の頂点lがa番目の集合, 右側の頂点rがb番目の集合に属する: 辺(l,r)が存在 $\implies$ a $\le$ b
  - 即ち DAG

|メンバ関数|概要|計算量|
|---|---|---|
|`DulmageMendelsohn(bg)`|コンストラクタ. <br> 引数は [`BipartiteGraph` クラス](BipartiteGraph.hpp)．| $O(E\sqrt{V})$|
|`size()`|分割された集合の個数(K+1)を返す．||
|`block(k)`| k 番目の集合を返す．<br> 頂点は昇順にソートされている．<br>(ので，ある区切りを境に左側頂点と右側頂点に分かれている)||
|`operator()(i)`|頂点 i の所属する集合が何番目かを返す．||
|`min_vertex_cover(ord={})`|最小頂点被覆を返す．<br> 引数は優先度で並んだ順列で，この順に貪欲に実行．<br> 引数を指定しない場合，頂点番号の低い順で実行．<br>（つまり左側の点が優先される）<br> 戻り値は `vector<int>` で最小頂点被覆に使う頂点の集合を表す．| $O(E+V)$|


## Verify
<details>
<summary>ここをクリックすると開きます</summary>

- [ ] [東京大学プログラミングコンテスト2013 K - 辞書順最小頂点被覆](https://atcoder.jp/contests/utpc2013/tasks/utpc2013_11) (辞書順最小頂点被覆のverify)

</details>