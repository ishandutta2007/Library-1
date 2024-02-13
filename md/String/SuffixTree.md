---
title: Suffix Tree (接尾辞木)
documentation_of: ../../src/String/SuffixTree.hpp
---
![suffixtree.svg](https://github.com/hashiryo/Library/blob/master/img/suffixtree.svg?raw=true)

## `SuffixTree` クラス

接尾辞木の頂点は辞書順にdfsのpre-orderでラベルを割り当てる.

|メンバ変数|概要|
|---|---|
|`graph`| 接尾辞木の[`Graph` クラス](../Graph/Graph.hpp) 表現.|
|`tree`| 接尾辞木の[重軽分解](../Graph/HeavyLightDecomposition.hpp)|

|メンバ関数|概要|計算量|
|---|---|---|
|`SuffixTree(sa, lcp)`|コンストラクタ. <br> 引数は [`SuffixArray` と `LCPArray`](src/String/SuffixArray.hpp) .|$\mathcal{O}(n)$ |
|`operator[](v)`| 接尾辞木上の頂点 v が指す矩形の情報を返す. <br> (suffix array の添字方向) $\times$ ( 高さ/ lcp 方向 )の矩形 $\lbrack l, r) \times \lbrack d, u)$ を意味する4つの整数 {l,r,d,u} を返す. ||
|`begin()`| 矩形情報配列の `begin()` を返す.||
|`end()`| 矩形情報配列の `end()` を返す||
|`size()`|接尾辞木の頂点数を返す.||
|`substr(l)`|接尾辞 $S_{l:}$ を構成する頂点のうちの末端の頂点を返す.|$\mathcal{O}(1)$|
|`substr(l,n)`|部分文字列 $S_{l:l+n}$ を構成する頂点のうちの末端の頂点を返す. |$\mathcal{O}(\log n)$|

## 問題例
[デンソークリエイトプログラミングコンテスト2022 Winter(AtCoder Beginner Contest 280) Ex - Substring Sort](https://atcoder.jp/contests/abc280/tasks/abc280_h)\
[HackerRank Ashton and String](https://www.hackerrank.com/challenges/ashton-and-string)\
[HackerRank String Function Calculation](https://www.hackerrank.com/challenges/string-function-calculation)

## 参考
[https://twitter.com/maspy_stars/status/1565901414236205057?s=20&t=S2Tu6ayozHcakxai8dmh4g](https://twitter.com/maspy_stars/status/1565901414236205057?s=20&t=S2Tu6ayozHcakxai8dmh4g)