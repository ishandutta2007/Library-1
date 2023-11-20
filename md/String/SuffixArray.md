---
title: Suffix Array (接尾辞配列)
documentation_of: ../../src/String/SuffixArray.hpp
---

$\newcommand{\SA}{\mathrm{SA}}$

## `SuffixArray<use_lcp_query=false, Int=int>` クラス
`use_lcp_query`: `bool` 値. `true` だと前処理時間とメモリが余計に増えるが関数 `lcp(i,j)` が使用可. \
（ `lcp(i,j)` では `SparseTable` を使うため）\
`Int`: 対象とする文字列のアルファベットを表現する型. 通常 `char` か `int`.\
（辞書順による比較のために `operator<` が定義されていて欲しい）

以下, 文字列 $S$ に対して構築することにする. \
文字列 $S$ の $k$ 文字目から開始した接尾辞を $S_{k:}$ と表すことにする.\
また $n = \lvert S\rvert$ とおく.

|メンバ変数|概要|
|---|---|
|`SA`| 接尾辞配列. <br> $\SA_k=i \Leftrightarrow$ 接尾辞を辞書順で並べたときの $k$ 番目 (0-index) が $S_{i:}$  <br>`vector<int>` 型でサイズは $n$.|
|`ISA`| 接尾辞のランクを表す配列.<br> $\mathrm{ISA}_i=k \Leftrightarrow \SA_k=i$ <br> `vector<int>` 型でサイズは $n$.|
|`LCP`|LCP配列. <br> $\mathrm{LCP}\lbrack k\rbrack:=$ $S_{\SA_k:}$ と $S_{\SA_{k+1}:}$ の共通接頭辞の長さ<br> `vector<int>` 型でサイズは $n-1$.|


|メンバ関数|概要|計算量|
|---|---|---|
|`SuffixArray(S)`|コンストラクタ. 文字列 $S$ を渡す. <br> 引数は `vector<Int>` か `string` を渡せる.|$\mathcal{O}(n)$ か $\mathcal{O}(n\log n)$|
|`operator[](i)`| `SA[i]` を返す.||
|`lcp(i,j)`|$S_{\SA_i:}$ と $S_{\SA_j:}$ の共通接頭辞の長さを返す. <br> クラステンプレートの第一引数が `true` でないと`static_assert` で落ちる. |$\mathcal{O}(1)$|
|`pattern_matching(P)`|文字列 $S$ の部分文字列でパターン文字列 $P$ と一致する箇所を探す.<br> 返り値は二つの非負整数 $l, r$ <br>s.t. $\lbrace \SA_k\;\vert\; l \leq k \lt r \rbrace = \lbrace i \;\vert\;P$ が $S_{i:}$ の接頭辞$\rbrace$ <br> ただし $P$ が $S$ の部分文字列でないとき $l=r$ .<br> 引数は `vector<Int>` か `string` を渡せる.|$\mathcal{O}(\lvert P\rvert \log n)$|

## 問題例
[AtCoder Beginner Contest 240 Ex - Sequence of Substrings](https://atcoder.jp/contests/abc240/tasks/abc240_h) (lcp, 部分文字列ソート, ロリハでTLE)