---
title: Suffix Array (接尾辞配列)
documentation_of: ../../src/String/SuffixArray.hpp
---

$\newcommand{\SA}{\mathrm{SA}}$
文字列 $S$ に対して構築することにする. \
文字列 $S$ の $k$ 文字目から開始した接尾辞を $S_{k:}$ と表すことにする.\
また $n = \lvert S\rvert$ とおく. 

## `SuffixArray<String>` クラス
` String`: 対象とする文字列を表現する型. `string` か `vector<int>` を想定.

接尾辞配列を $\SA$ とおく.\
$\SA_k=i \Leftrightarrow$ 接尾辞を辞書順で並べたときの $k$ 番目 (0-index) が $S_{i:}$ 
|メンバ変数|概要|
|---|---|
|`sa`| 接尾辞配列 $\SA$. <br>`vector<int>` 型でサイズは $n$.|
|`s`| 元の文字列.  <br> `String` 型でサイズは $n$.|

|メンバ関数|概要|計算量|
|---|---|---|
|`SuffixArray(S)`|コンストラクタ. 文字列 $S$ を渡す. <br> 引数は通常 `vector<T>` か `string` を想定.|$O(n)$ か $O(n\log n)$|
|`operator[](k)`| $\SA_k$ を返す.||
|`begin()`|$\SA$ の `begin()` を返す.||
|`end()`|$\SA$ の `end()` を返す||
|`size()`|$n$ を返す.||
|`pattern_matching(P)`|文字列 $S$ の部分文字列でパターン文字列 $P$ と一致する箇所を探す.<br> 返り値は二つの非負整数 $l, r$ <br>s.t. $\lbrace \SA_k\;\vert\; l \leq k \lt r \rbrace = \lbrace i \;\vert\;P$ が $S_{i:}$ の接頭辞$\rbrace$ <br> ただし $P$ が $S$ の部分文字列でないとき $l=r$ .<br> 引数は `String`.|$O(\lvert P\rvert \log n)$|

## `LCPArray` クラス
$\mathrm{LCP}_k:=$ $S_{\SA_k:}$ と $S_{\SA_{k+1}:}$ の共通接頭辞の長さ

|メンバ変数|概要|
|---|---|
|`rnk`| 配列 $\mathrm{rank}$. <br> $\mathrm{rank}_{i}=k\Leftrightarrow \SA_k=i$ <br>`vector<int>` 型でサイズは $n$.|

|メンバ関数|概要|計算量|
|---|---|---|
|`LCPArray(sa)`|コンストラクタ.<br> `SuffixArray`クラスを渡す.|$O(n\log n)$|
|`operator[](k)`| $\mathrm{LCP}_k$ を返す.||
|`begin()`| $\mathrm{LCP}$ の `begin()` を返す.||
|`end()`|$\mathrm{LCP}$ の `end()` を返す||
|`lcp(i,j)`|$S_{\SA_i:}$ と $S_{\SA_j:}$ の共通接頭辞の長さを返す. |$O(1)$|
## Verify
<details>
<summary>ここをクリックすると開きます</summary>

<input disabled="" type="checkbox"> [DISCO presents ディスカバリーチャンネル プログラミングコンテスト2016 予選 C - アメージングな文字列は、きみが作る！](https://atcoder.jp/contests/discovery2016-qual/tasks/discovery_2016_qual_c)\
<input disabled="" type="checkbox"> [DISCO presents ディスカバリーチャンネル コードコンテスト2020 本戦 C - Smaller-Suffix-Free Sequences](https://atcoder.jp/contests/ddcc2020-final/tasks/ddcc2020_final_c)

</details>