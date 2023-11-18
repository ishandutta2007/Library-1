---
title: Rolling-Hash
documentation_of: ../../src/String/RollingHash.hpp
---
## `RollingHash<K, Int=int>` クラス

`K`: 演算子オーバーロード `+`, `-`, `*` が定義されているクラス.\
（衝突確率のことを考慮しても有限体をなす代数的構造であることが望ましい. ） \
`Int`: 対象とする文字列のアルファベットを表現する型. 通常 `char` か `int`.\
（辞書順による比較のために `operator<` が定義されていて欲しい）


|`static` 関数|概要|
|---|---|
|`init(b)`|基底 $b$ をセットして初期化する. 最初に必ず呼ぶ.|
|`base_pow(i)`|$b^i$ を返す.|

|メンバ関数|概要|
|---|---|
|`RollingHash(s)`|コンストラクタ. 文字列 $S$ を渡す. <br> 引数のクラスは`vector<Int>`か`string` を想定している.|
|`length()`|文字列の長さを返す.|
|`hash()`|文字列を指すハッシュ値を返す.|
|`sub(int b,int n)`|部分文字列 $S\lbrack b:b+n\rbrack$ に対応する `RollingHash` クラスのインスタンスを返す. |
|`to_str()`|文字列に直して`string`で返す. デバッグ用.|

|演算子オーバーロード|概要|計算量|
|---|---|---|
|`operator==(r)`|文字列が等しいなら `true`.|$\mathcal{O}(1)$|
|`operator!=(r)`|文字列が等しくないなら `true`.|$\mathcal{O}(1)$|
|`operator<(r)`|左辺が右辺より辞書順で小さいなら `true`. |$\mathcal{O}(\log n)$<br> ただし $n=\min(\lvert S_l\rvert,\lvert S_r\rvert)$|

|関数|概要|
|---|---|
|`concat_hash(rh...)`| 複数の `RollingHash` クラスのインスタンスを与える. <br> 対応する文字列をその順にconcatしたときのハッシュ値を返す.|
|`lcp(l,r)`|2つの `RollingHash` クラスのインスタンスを与える. <br>対応する2つの文字列のLCP(最長共通接頭辞; Longest Common Prefix)の長さを返す.|
|`lcs(l,r)`|2つの `RollingHash` クラスのインスタンスを与える. <br>対応する2つの文字列の最長共通接尾辞(Longest Common Suffix)の長さを返す.|
|`concat_cmp(l,r)`|2つの `RollingHash` クラスのインスタンスを与える. <br>対応する2つの文字列 $S_l,S_r$ についてconcatの前後を変えた際辞書順比較 $S_lS_r < S_rS_l$ (`bool`値) を返す .|


### 備考
比較が $\mathcal{O}(1)$ じゃないので sort をするときは [stable_sort](https://cpprefjp.github.io/reference/algorithm/stable_sort.html) を使うと良さげ.



## 問題例
[AtCoder Regular Contest 050 D - Suffix Concat](https://atcoder.jp/contests/arc050/tasks/arc050_d) (concat_cmp)\
[AtCoder Regular Contest 055 C - ABCAC](https://atcoder.jp/contests/arc055/tasks/arc055_c) \
[AtCoder Beginner Contest 274 Ex - XOR Sum of Arrays](https://atcoder.jp/contests/abc274/tasks/abc274_h) (標数2の体 e.g. Nimber)\
[AtCoder Beginner Contest 284 F - ABCBAC](https://atcoder.jp/contests/abc284/tasks/abc284_f) (sp judge)\
[競プロ典型 90 問 047 - Monochromatic Diagonal（★7）](https://atcoder.jp/contests/typical90/tasks/typical90_au)\
[早稲田大学プログラミングコンテスト2019 E - Artist](https://atcoder.jp/contests/wupc2019/tasks/wupc2019_e)