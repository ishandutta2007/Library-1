---
title: Rolling-Hash
documentation_of: ../../src/String/RollingHash.hpp
---
## `RollingHash<K, Int=int>` クラス

`K`: 演算子オーバーロード `+`, `-`, `*` が定義されているクラスが乗せられる.\
（衝突確率のことを考慮しても有限体をなす代数的構造であることが望ましい. ） \
`Int`: 対象とする文字列のアルファベットを表現する型. 通常 `char` か `int`.\
（辞書順による比較のために `operator<` が定義されていて欲しい）


#### `static` 関数

|||
|---|---|
|`init(b)`|基底 $b$ をセットして初期化する. 最初に必ず呼ぶ.|
|`base_pow(i)`|$b^i$ を返す.|


#### メンバ　関数
|||
|---|---|
|`RollingHash(s)`|コンストラクタ. <br> 引数のクラスは`vector<Int>`か`string` を想定している.|
|`init(b)`||



## 問題例
[AtCoder Regular Contest 050 D - Suffix Concat](https://atcoder.jp/contests/arc050/tasks/arc050_d) (concat_cmp)\
[AtCoder Regular Contest 055 C - ABCAC](https://atcoder.jp/contests/arc055/tasks/arc055_c) \
[AtCoder Beginner Contest 274 Ex - XOR Sum of Arrays](https://atcoder.jp/contests/abc274/tasks/abc274_h) (標数2の体 e.g. Nimber)\
[AtCoder Beginner Contest 284 F - ABCBAC](https://atcoder.jp/contests/abc284/tasks/abc284_f) (sp judge)\
[競プロ典型 90 問 047 - Monochromatic Diagonal（★7）](https://atcoder.jp/contests/typical90/tasks/typical90_au)\
[早稲田大学プログラミングコンテスト2019 E - Artist](https://atcoder.jp/contests/wupc2019/tasks/wupc2019_e)