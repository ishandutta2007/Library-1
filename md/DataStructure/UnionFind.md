---
title: Union-Find
documentation_of: ../../src/DataStructure/UnionFind.hpp
---

## `UnionFind<undoable=false>` クラス

1. テンプレート引数を指定しない，もしくは `false`　の場合
   - undo 操作が不可能
   - 経路圧縮により高速
2. テンプレート引数が `true` の場合
   - undo 操作が可能

アッカーマン関数の逆関数を $\alpha(n)$ と表す．

|メンバ関数|概要|計算量|
|---|---|---|
|`UnionFind(n)`|コンストラクタ. 要素数 $n$ を渡す.||
|`size(x)`|要素 $x$ の属する集合のサイズを返す.||
|`root(x)`|要素 $x$ の属する集合の代表元を返す.|1. $O(\alpha(n))$ <br> 2. $O(\log n)$|
|`same(x,y)`|要素 $x,y$ が同じ集合に属していれば `true` を返す．|1. $O(\alpha(n))$ <br> 2. $O(\log n)$|
|`unite(x,y)`|要素 $x,y$ それぞれが属する集合を併合する．<br> 元々要素 $x,y$ がそれぞれ異なる集合に属していれば，`true` を返す. |1. $O(\alpha(n))$ <br> 2. $O(\log n)$|
|`time()`|この関数を読んだ時点でのタイムスタンプを返す．<br> (2.) のときのみ|$O(1)$|
|`undo()`|`unite`によって状態が変化したうちの最も新しい場合を無かったことにする．<br> (2.) のときのみ|$O(1)$|
|`rollback(t)`| タイムスタンプ $t$ のときまで `undo` する． <br> (2.) のときのみ|`undo` を呼ぶ回数に比例|