---
title: Suffix Array (接尾辞配列)
documentation_of: ../../src/String/SuffixArray.hpp
---

$\newcommand{\SA}{\mathrm{SA}}$

## `SuffixArray<use_lcp_query=false, Int=int>` クラス
`use_lcp_query`: bool値. true だと前処理時間とメモリが余計に増えるが関数 `lcp(i,j)` が使用可. \
（ `lcp(i,j)` では `SparseTable` を使うため）\
`Int`: 対象とする文字列のアルファベットを表現する型. 通常 `char` か `int`.\
（辞書順による比較のために `operator<` が定義されていて欲しい）

以下, 文字列 $S$ に対して構築することにする. また $n = \lvert S\rvert$ とする.

|メンバ変数|概要|
|---|---|
|`SA`| 接尾辞配列. <br> $\SA_i=k \Leftrightarrow$ 接尾辞を辞書順で並べたときの $i$ 番目 (0-index) が $S\lbrack k:\rbrack$  <br>`vector<int>` 型でサイズは $n$.|
|`ISA`| 接尾辞のランクを表す配列.<br> $\mathrm{ISA}_k=i \Leftrightarrow \SA_i=k$ <br> `vector<int>` 型でサイズは $n$.|
|`LCP`|LCP配列. <br> $\mathrm{LCP}\lbrack i\rbrack\stackrel{\mathrm{def}}{=}$ $S\lbrack \SA_i: \rbrack$ と $S\lbrack \SA_{i+1}: \rbrack$ の共通接頭辞の長さ<br> `vector<int>` 型でサイズは $n-1$.|


|メンバ関数|概要|計算量|
|---|---|---|
|`SuffixArray(S)`|コンストラクタ. <br> 引数は `vector<Int>` か `string` を渡せる.|$\mathcal{O}(n)$ か $\mathcal{O}(n\log n)$|
|`operator[](i)`| `SA[i]` を返す.||
|`size()`|$S$ の長さ $n$ を返す.||
|`lcp(i,j)`|$S\lbrack \SA_i: \rbrack$ と $S\lbrack \SA_j: \rbrack$ の共通接頭辞の長さを返す.|$\mathcal{O}(1)$|
|`pattern_matching(P)`|文字列 $S$ の部分文字列でパターン文字列 $P$ と一致する箇所を探す.<br> 返り値は二つの非負整数 $l, r$ <br>s.t. $\lbrace \SA_i\;\vert\; l \leq i \lt r \rbrace = \lbrace k \;\vert\;P$ が $S\lbrack k:\rbrack$ の接頭辞$\rbrace$ <br> ただし $P$ が $S$ の部分文字列でないとき $l=r$ .<br> 引数は `vector<Int>` か `string` を渡せる.|$\mathcal{O}(\lvert P\rvert \log n)$|

## 問題例
[AtCoder Beginner Contest 240 Ex - Sequence of Substrings](https://atcoder.jp/contests/abc240/tasks/abc240_h) (lcp, 部分文字列ソート, ロリハでTLE)