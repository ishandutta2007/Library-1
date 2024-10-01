---
title: Z-Algorithm
documentation_of: ../../src/String/z_algorithm.hpp
---

文字列 $S$ に対して構築することにする. \
文字列 $S$ の $l$ 文字目から $r-1$ 文字目までの部分文字列を $S_{l:r}$ と表すことにする.\
文字列 $S$ の $k$ 文字目から開始した接尾辞を $S_{k:}$ と表すことにする.\
文字列 $S$ の 長さ $k$ の接頭辞を $S_{:k}$ と表すことにする.\
また $n = \lvert S\rvert$ とおく. 

|関数|概要|計算量|
|---|---|---|
|`z_algorithm(S)`| 長さ $n$ の配列 $Z$ を返す. <br> $Z_k := $ $S$ と接尾辞 $S_{k:}$ の最長共通接頭辞の長さ. <br> 引数は `vector<T>` か `string` を想定.|$O(n)$|
|`z_to_kmp(z)`| `z_algorithm` の配列 $Z$ から kmp 法の配列 $\mathrm{KMP}$ に変換して返す. <br> $\mathrm{KMP} _k:=$ 接頭辞 $S _{:k}$ の最長 [**境界 (border)**](https://www.iss.is.tohoku.ac.jp/stringology/Algorithms/Basic/border.html) の長さ. |$O(n)$|
|`run_enumerate(S)`|文字列 $S$ の [**連 (run)**](https://www.iss.is.tohoku.ac.jp/stringology/Algorithms/Basic/run.html) を列挙する. <br> タプル $(t,l,r)$ で部分文字列 $S_{l:r}$ の最小周期が $t$ であることを表現する. <br> 返り値は `vector<tuple<int,int,int>>`. <br> 引数は `vector<T>` か `string` を想定.|$O(n\log n)$|
