---
title: Incremental-Bridge-Connectivity (二辺連結成分)
documentation_of: ../../src/Graph/IncrementalBridgeConnectivity.hpp
---

## `IncrementalBridgeConnectivity` クラス

|メンバ関数|概要|
|---|---|
|`IncrementalBridgeConnectivity(n)`|コンストラクタ. <br> 頂点の数 $n$ を渡す．|
|`add_edge(u,v)`|辺 $(u,v)$ を追加する．|
|`leader(v)`|頂点 $v$ が属する二辺連結成分の代表頂点を返す．|
|`size(v)`|頂点 $v$ が属する二辺連結成分のサイズを返す．|
|`two_edge_connected(u,v)`|頂点 $u,v$ が共通の二辺連結成分に属するなら `true`, それ以外なら `false` を返す．|
|`connected(u,v)`|頂点 $u,v$ が連結なら `true`, それ以外なら `false` を返す．|

## Verify
<details>
<summary>ここをクリックすると開きます</summary>

<input disabled type="checkbox"> [AtCoder Regular Contest 039 D - 旅行会社高橋君](https://atcoder.jp/contests/arc039/tasks/arc039_d)

</details>

## 参考
[https://scrapbox.io/data-structures/Incremental_Bridge-Connectivity](https://scrapbox.io/data-structures/Incremental_Bridge-Connectivity)