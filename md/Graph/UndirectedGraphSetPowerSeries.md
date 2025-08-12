---
title: 無向グラフ数え上げ(集合冪級数)
documentation_of: ../../src/Graph/UndirectedGraphSetPowerSeries.hpp
---

グラフ $G(V,E)$ について扱う. 
1. $S\subseteq V$ を頂点集合とする**誘導**部分グラフの "ほげ" の個数を $f_S$ とするとき,  $f$ が「ほげの個数を表す集合冪級数」であるとする.
2. $S\subseteq V$ を頂点集合とする部分グラフのうちそれが "ほげグラフ" であるものの個数を $f_S$ とするとき, $f$ が「ほげグラフを表す集合冪級数」であるとする. 

## メンバ関数

※ `selfloop_num()`, `tutte_polynomial(x,y)` 以外は自己ループは無視する. \
※ 多重辺は考慮する.

|名前|概要|計算量|
|---|---|---|
|`UndirectedGraphSetPowerSeries(n)`|コンストラクタ. <br> 頂点数 $n$ を渡す| 以下 $n=\lvert V\rvert$ とする.|
|`UndirectedGraphSetPowerSeries`<br>`(vector<vector<int>> g)`|コンストラクタ. <br>隣接行列を渡す.||
|`add_edge(u,v,c=1)`|無向辺 $(u,v)$ を $c$ 本追加する.||
|`operator[](u)`|隣接行列の第u行目を返す.||
|`only_connected(f)`| 集合冪級数 $f$ が表すグラフについて, そのうち連結してるものだけに制限したグラフ (e.g. 森 → 木) を表す集合冪級数を返す. <br> $f_{\varnothing}=1$ でないと assert で落ちる.      |$O(n^22^n)$|
|`disjoint_union(f)`|集合冪級数 $f$ が表すグラフについて, それを [disjoint union](https://en.wikipedia.org/wiki/Disjoint_union_of_graphs) (e.g. 木 → 森) したグラフを表す集合冪級数を返す. <br> $f$ が表すグラフは連結であることを想定している. <br> $f_{\varnothing}=0$ でないと assert で落ちる. |$O(n^22^n)$|
|`only_biconnected(f)`|集合冪級数 $f$ が表すグラフについて, そのうち2頂点連結してるものだけに制限したグラフを表す集合冪級数を返す. <br> $f$ が表すグラフは連結であることを想定している. <br> 各頂点 $v\in V$ について $f_{\lbrace v\rbrace}=1$ でないと assert で落ちる.  |$O(n^32^n)$|
|`articulation_union(f)`|集合冪級数 $f$ が表すグラフについて, それを関節点で結合したグラフを表す集合冪級数をを返す. <br> $f$ が表すグラフは2頂点連結であることを想定している. <br> 各頂点 $v\in V$ について $f_{\lbrace v\rbrace}=0$ でないと assert で落ちる.  |$O(n^32^n)$|
|`only_2edge_connected(f)`|集合冪級数 $f$ が表すグラフについて, そのうち2辺連結してるものだけに制限したグラフを表す集合冪級数をを返す. <br> $f$ が表すグラフは連結であることを想定している. |$O(n^32^n)$|
|`bridge_union(f)`|集合冪級数 $f$ が表すグラフについて, それを橋で結合したグラフを表す集合冪級数をを返す. <br> $f$ が表すグラフは2辺連結であることを想定している. |$O(n^32^n)$|
|`edge_num()`|辺空間の基底の数 (辺の本数) を表す集合冪級数を返す.|$O(n2^n)$|
|`connected_component_num()`|連結成分の個数を表す集合冪級数を返す.|$O(n2^n)$|
|`cycle_space_rank()`|[サイクル基底](https://en.wikipedia.org/wiki/Cycle_basis)の数を表す集合冪級数を返す.|$O(n2^n)$|
|`selfloop_num()`|自己ループの数を表す集合冪級数を返す.|$O(2^n)$|
|`odd_deg_num()`|奇数次数の頂点の数を表す集合冪級数を返す.|$O(n^22^n)$|
|`space_size<T>(rank)`|$\mathbb{F}_2$ベクトル空間の基底の数を表す集合冪級数 $f^{\rm rank}$ を渡して, その空間の元の個数を表す集合冪級数を返す.|$O(2^n)$|
|`graph<T>()`|辺空間の元の個数を表す集合冪級数 (任意のグラフを表す集合冪級) を返す.|$O(n2^n)$|
|`cycle_space_size<T>()`|サイクル空間の元の個数を表す集合冪級数 (全ての頂点の次数が偶数のグラフを表す集合冪級数) を返す. |$O(n2^n)$|
|`connected_graph<T>()`|連結グラフを表す集合冪級数を返す.|$O(n^22^n)$|
|`eulerian_graph<T>()`|[オイラーグラフ](https://ja.wikipedia.org/wiki/%E3%82%AA%E3%82%A4%E3%83%A9%E3%83%BC%E8%B7%AF) (オイラー閉路をもつグラフ) を表す集合冪級数を返す.|$O(n^22^n)$|
|`connected_biparate_graph<T>()`|連結二部グラフを表す集合冪級数を返す.|$O(n^22^n)$|
|`tree<T>()`|木を表す集合冪級数を返す.|$O(n^22^n)$|
|`forest<T>()`|森を表す集合冪級数を返す.|$O(n^22^n)$|
|`cycle_graph<T>()`|[閉路グラフ](https://ja.wikipedia.org/wiki/%E9%96%89%E8%B7%AF%E3%82%B0%E3%83%A9%E3%83%95)を表す集合冪級数を返す.|$O(n^22^n)$|
|`biconnected_graph<T>()`|2頂点連結グラフを表す集合冪級数を返す.|$O(n^32^n)$|
|`two_edge_connected_graph<T>()`|2辺連結グラフを表す集合冪級数を返す.|$O(n^32^n)$|
|`cactus_graph<T>()`|[カクタスグラフ](https://ja.wikipedia.org/wiki/%E3%82%AB%E3%82%AF%E3%82%BF%E3%82%B9%E3%82%B0%E3%83%A9%E3%83%95)を表す集合冪級数を返す.|$O(n^32^n)$|
|`acyclic_orientations<T>()`|[acyclic orientations](https://en.wikipedia.org/wiki/Acyclic_orientation) (DAGにするような向き付け) の個数を表す集合冪級数を返す.|$O(n^22^n)$|
|`colorings_using_exactly_k_colors_num<T>()`|グラフ $G$ について要素数が $n+1$ 個の配列 `a` を返す.<br> `a[k]` はちょうど $k$ 色を使った頂点彩色の組み合わせ数である.  |$O(n^22^n)$|
|`chromatic_polynomial<T>()`|グラフ $G$ の[彩色多項式](https://en.wikipedia.org/wiki/Chromatic_polynomial) $P(G,x)=\sum_{i=0}^na_ix^i$ (の係数) を返す. <br> $n=0$ の場合は`vector({0})`を返す.|$O(n^22^n)$|
|`tutte_polynomial<T>(x,y)`|グラフ $G$ の[Tutte多項式](https://en.wikipedia.org/wiki/Tutte_polynomial)に対して $x,y$ を代入した値を返す. |$O(n^22^n)$|
|`perfect_matching<T>()`|グラフ $G$ の完全マッチングの組み合わせ数を返す. |$O(n^22^{n/2})$|
|`all_matching<T>()`|グラフ $G$ の（完全とは限らない）マッチングの組み合わせ数を返す. |$O(n^22^{n/2})$|
|`k_matching<T>()`|グラフ $G$ について要素数が $\lfloor n/2\rfloor+1$ 個の配列 `a` を返す. <br> `a[k]` はちょうど $k$ 本のマッチングがある組み合わせ数である.|$O(n^22^{n/2})$|


## 参考
[https://github.com/EntropyIncreaser/ioi2021-homework/blob/master/thesis/main.tex](https://github.com/EntropyIncreaser/ioi2021-homework/blob/master/thesis/main.tex)

## Verify
<details>
<summary>ここをクリックすると開きます</summary>

<input disabled type="checkbox"> [LibreOJ #155. Tutte 多项式](https://loj.ac/p/155) (Tutte 多項式)<br>
<input disabled type="checkbox"> [LibreOJ #3165. 「CEOI2019」游乐园](https://loj.ac/p/3165) (acyclic_orientations (Tutte 多項式, 彩色多項式))<br>
<input disabled type="checkbox"> [LibreOJ #6673. EntropyIncreaser 与山林](https://loj.ac/p/6673) (オイラーグラフ)<br>
<input disabled type="checkbox"> [LibreOJ #6719. 「300iq Contest 2」数仙人掌 加强版](https://loj.ac/p/6719) (カクタスグラフ)<br>
<input disabled type="checkbox"> [LibreOJ #6729. 点双连通生成子图计数](https://loj.ac/p/6729) (二重点連結グラフ)<br>
<input disabled type="checkbox"> [LibreOJ #6730. 边双连通生成子图计数](https://loj.ac/p/6730) (二重辺連結グラフ)<br>
<input disabled type="checkbox"> [LibreOJ #6787. 色多项式](https://loj.ac/p/6787) (彩色多項式)<br>
<input disabled type="checkbox"> [LibraOJ #2340. 「WC2018」州区划分](https://loj.ac/p/2340)<br>
<input disabled type="checkbox"> [Xmas Contest 2022 F - Fast as Fast as Ryser](https://atcoder.jp/contests/xmascon22/tasks/xmascon22_f) (k-matchig)<br>
<input disabled type="checkbox"> [Yuhao Du Contest 7 F. Fast as Ryser](https://qoj.ac/contest/449/problem/2068) (k-mathcing)

</details>