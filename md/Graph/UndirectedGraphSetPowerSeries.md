---
title: 無向グラフの数え上げ (集合冪級数)
documentation_of: ../../src/Graph/UndirectedGraphSetPowerSeries.hpp
---

## 概要

無向グラフ $G=(V, E)$ の、頂点集合 $S \subseteq V$ によって定まる**誘導部分グラフ**が持つ様々な性質（連結性、辺の数、彩色多項式など）を、集合冪級数 (Set Power Series, SPS) を用いて数え上げるためのライブラリ。

集合冪級数 $f$ は `std::vector<T>` で表現され、その添え字は頂点集合を表すビットマスクに対応する。係数 $f_S$ (実装上は `f[maskS]`) は、頂点集合 $S$ 上の誘導部分グラフに関する何らかの特性値（例: その性質を持つグラフの個数、辺の本数、期待値など）を表す。

テンプレートパラメータ `T` は、係数の型を表す。`ModInt` のような四則演算が定義された型を想定している。

### 注意事項
- `selfloop_num()`, `tutte_polynomial(x,y)` を除き、自己ループは無視される。
- 多重辺は辺の数として考慮される。コンストラクタや `add_edge` で `g[u][v]` や `c` に辺の多重度を指定できる。

## メンバ関数

|名前|概要|計算量|
|---|---|---|
|`UndirectedGraphSetPowerSeries(n)`|頂点数 $n$ のグラフを構築する。|$O(n^2)$|
|`UndirectedGraphSetPowerSeries`<br>`(g)`|隣接行列 `g` からグラフを構築する。`g[i][j]` は辺 $(i,j)$ の多重度を表す。| $O(n^2)$ |
|`add_edge(u,v,c=1)`|無向辺 $(u,v)$ を $c$ 本（多重度 $c$ で）追加する。||
|`operator[](u)`|隣接行列の第 $u$ 行への読み取り専用アクセス。||
|`only_connected(f)`| ある性質を持つ誘導部分グラフの個数を表すSPS $f$ から、そのうち連結なものだけを数えるSPS $g$ を計算する (e.g. 森 → 木)。これは $g = \log(f)$ に対応する。入力 $f$ は $f_{\varnothing}=1$ を満たす必要がある。 |$O(n^22^n)$|
|`disjoint_union(f)`|連結な誘導部分グラフの個数を表すSPS $f$ から、それらの非交和として構成されるグラフ (e.g. 木 → 森) の個数を表すSPS $g$ を計算する。これは $g = \exp(f)$ に対応する。入力 $f$ は $f_{\varnothing}=0$ を満たす必要がある。|$O(n^22^n)$|
|`only_biconnected(f)`|連結グラフのSPS $f$ から、2頂点連結なものだけを数えるSPSを返す。$f$ は各頂点 $v$ について $f_{\{v\}}=1$ を満たす必要がある。|$O(n^32^n)$|
|`articulation_union(f)`|2頂点連結グラフのSPS $f$ から、それらを関節点で結合して得られるグラフのSPSを返す。$f$ は各頂点 $v$ について $f_{\{v\}}=0$ を満たす必要がある。|$O(n^32^n)$|
|`only_2edge_connected(f)`|連結グラフのSPS $f$ から、2辺連結なものだけを数えるSPSを返す。|$O(n^32^n)$|
|`bridge_union(f)`|2辺連結グラフのSPS $f$ から、それらを橋で結合して得られるグラフのSPSを返す。|$O(n^32^n)$|
|`edge_num()`|各頂点集合 $S$ について、その誘導部分グラフの辺数 $|E_S|$ を係数に持つSPSを返す。|$O(n2^n)$|
|`connected_component_num()`|各頂点集合 $S$ について、その誘導部分グラフの連結成分数を係数に持つSPSを返す。|$O(n2^n)$|
|`cycle_space_rank()`|各頂点集合 $S$ について、その誘導部分グラフの[サイクル基底](https://en.wikipedia.org/wiki/Cycle_basis)の次元 (辺数 - 頂点数 + 連結成分数) を係数に持つSPSを返す。|$O(n2^n)$|
|`selfloop_num()`|各頂点集合 $S$ について、その誘導部分グラフの自己ループの総数を係数に持つSPSを返す。|$O(2^n)$|
|`odd_deg_num()`|各頂点集合 $S$ について、その誘導部分グラフ内で奇数次数を持つ頂点の数を係数に持つSPSを返す。|$O(n^22^n)$|
|`space_size<T>(rank)`|$\\mathbb{F}_2$ ベクトル空間の次元を表すSPS `rank` から、その空間の元の個数 ($2^{\text{rank}}$) を表すSPSを返す。|$O(2^n)$|
|`graph<T>()`|各頂点集合 $S$ について、その誘導部分グラフの総数 ($2^{\lvert E_S \rvert}$) を係数に持つSPSを返す。|$O(n2^n)$|
|`cycle_space_size<T>()`|各頂点集合 $S$ について、その誘導部分グラフのうち全ての頂点次数が偶数であるものの数 ($2^{\text{cycle\_rank}}$) を係数に持つSPSを返す。|$O(n2^n)$|
|`connected_graph<T>()`|連結な誘導部分グラフの数を表すSPSを返す。|$O(n^22^n)$|
|`eulerian_graph<T>()`|[オイラーグラフ](https://ja.wikipedia.org/wiki/%E3%82%AA%E3%82%A4%E3%83%A9%E3%83%BC%E8%B7%AF) (連結かつ全頂点が偶数次数) である誘導部分グラフの数を表すSPSを返す。|$O(n^22^n)$|
|`connected_biparate_graph<T>()`|連結な二部グラフである誘導部分グラフの数を表すSPSを返す。|$O(n^22^n)$|
|`tree<T>()`|木である誘導部分グラフの数を表すSPSを返す。|$O(n^22^n)$|
|`forest<T>()`|森である誘導部分グラフの数を表すSPSを返す。|$O(n^22^n)$|
|`cycle_graph<T>()`|[閉路グラフ](https://ja.wikipedia.org/wiki/%E9%96%89%E8%B7%AF%E3%82%B0%E3%83%A9%E3%83%95)である誘導部分グラフの数を表すSPSを返す。|$O(n^22^n)$|
|`biconnected_graph<T>()`|2頂点連結な誘導部分グラフの数を表すSPSを返す。|$O(n^32^n)$|
|`two_edge_connected_graph<T>()`|2辺連結な誘導部分グラフの数を表すSPSを返す。|$O(n^32^n)$|
|`cactus_graph<T>()`|[カクタスグラフ](https://ja.wikipedia.org/wiki/%E3%82%AB%E3%82%AF%E3%82%BF%E3%82%B9%E3%82%B0%E3%83%A9%E3%83%95)である誘導部分グラフの数を表すSPSを返す。|$O(n^32^n)$|
|`acyclic_orientations<T>()`|各頂点集合 $S$ について、その誘導部分グラフの[無閉路な向き付け](https://en.wikipedia.org/wiki/Acyclic_orientation)の数を係数に持つSPSを返す。|$O(n^22^n)$|
|`colorings_using_exactly_k_colors_num<T>()`|グラフ全体 $G(V,E)$ について、ちょうど $k$ 色を用いる頂点彩色の数を `a[k]` に格納した配列 `a` を返す。|$O(n^22^n)$|
|`chromatic_polynomial<T>()`|グラフ全体 $G(V,E)$ の[彩色多項式](https://en.wikipedia.org/wiki/Chromatic_polynomial) $P(G,x)=\sum_{i=0}^n a_i x^i$ の係数配列を返す。|$O(n^22^n)$|
|`tutte_polynomial<T>(x,y)`|グラフ全体 $G(V,E)$ の[Tutte多項式](https://en.wikipedia.org/wiki/Tutte_polynomial) $T_G(x,y)$ の値を返す。|$O(n^22^n)$|
|`perfect_matching<T>()`|グラフ全体 $G(V,E)$ の完全マッチングの個数（重み付き）を返す。※1|$O(n^22^{n/2})$|
|`all_matching<T>()`|グラフ全体 $G(V,E)$ のマッチング（完全とは限らない）の個数（重み付き）を返す。※1|$O(n^22^{n/2})$|
|`k_matching<T>()`|グラフ全体 $G(V,E)$ について、サイズ $k$ のマッチングの個数（重み付き）を `a[k]` に格納した配列 `a` を返す。※1|$O(n^22^{n/2})$|

※1: マッチング系の関数の計算量は、分割統治的なアルゴリズムにより、頂点数を約半分にした $\lceil n/2 \rceil$ 個の頂点に関する問題に帰着させているため、$2^n$ ではなく $2^{n/2}$ に比例する時間となります。

## 参考
- [https://github.com/EntropyIncreaser/ioi2021-homework/blob/master/thesis/main.tex](https://github.com/EntropyIncreaser/ioi2021-homework/blob/master/thesis/main.tex)

## Verify

- [Xmas Contest 2022 F - Fast as Fast as Ryser](https://atcoder.jp/contests/xmascon22/tasks/xmascon22_f) (k_matching)
- [Yuhao Du Contest 7 F. Fast as Ryser](https://qoj.ac/contest/449/problem/2068) (k_matching)
- [Library Checker - Hafnian of Matrix](https://judge.yosupo.jp/problem/hafnian_of_matrix) (perfect_matching)
- [AtCoder ABC213 G - Connectivity 2](https://atcoder.jp/contests/abc213/tasks/abc213_g) (connected_graph)