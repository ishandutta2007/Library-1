---
title: Aho-Corasick
documentation_of: ../../src/String/AhoCorasick.hpp
---
## `AhoCorasick<String>` クラス
pattern と suffix がマッチする文字列を受理するDFA. \
オートマトンの状態は `int` で表現する.

|メンバクラス|概要|
|---|---|
|`symbol_type`|アルファベットを表すクラス.|

|メンバ関数|概要|
|---|---|
|`AhoCorasick(P)`|コンストラクタ. <br>パターン文字列の集合 $\mathcal{P}$ を渡す. <br> 引数の型は `vector<String>`.<br> 計算量は $O\left(\sum_{P\in \mathcal{P}}\lvert P\rvert\right)$.|
|`initial_state()`|オートマトンの初期状態を意味する整数を返す.|
|`matched_patterns(s)`|オートマトンの状態 $s$ の時にマッチしているパターン文字列を全て返す. <br> 実際には $\mathcal{P}=\lbrace P_i\rbrace$ の中の何番目かを指す添字 $i$ の集合を返す. <br> 返り値は`vector<int>`.|
|`is_accept(s)`|オートマトンの状態 $s$ が受理状態 (マッチしているパターン文字列が存在する) なら `true` を返す. |
|`transition(s, c)`|オートマトンの状態 $s$ の時にアルファベット $c$ が与えられた際の遷移先の状態を返す. <br> $c$ の型は `symbol_type`|
|`state_size()`|オートマトンの状態の総数を返す.|

## Verify


- [天下一プログラマーコンテスト2016本戦（オープンコンテスト） C - たんごたくさん](https://atcoder.jp/contests/tenka1-2016-final-open/tasks/tenka1_2016_final_c)
- [2010年 日本情報オリンピック春合宿OJ dna - DNAの合成](https://atcoder.jp/contests/joisc2010/tasks/joisc2010_dna)


