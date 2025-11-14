---
title: Union-Find (undo 可能)
documentation_of: ../../src/DataStructure/UnionFind_Undoable.hpp
---

## `UnionFind_Undoable`　クラス
undo可能な Union-Find．\
状態が変化しなくても `unite` を一回呼ぶごとにタイムスタンプが増える．\
経路圧縮をしないため計算量は $\log$ になる．

|メンバ関数|概要|計算量|
|---|---|---|
|`UnionFind(n)`|コンストラクタ. 要素数 $n$ を渡す.||
|`size(u)`|要素 $u$ の属する集合のサイズを返す.||
|`leader(u)`|要素 $u$ の属する集合の代表元を返す.|$O(\log n)$|
|`connected(u,v)`|要素 $u,v$ が同じ集合に属していれば `true` を返す．|$O(\log n)$|
|`unite(u,v)`|要素 $u,v$ それぞれが属する集合を併合する．<br> すでに要素 $u,v$ が同じ集合に属していれば，`false` を返す． |$O(\log n)$|
|`time()`|この関数を読んだ時点でのタイムスタンプを返す．最初は0．|$O(1)$|
|`undo()`|直近の `unite` 操作を無かったことにする．|$O(1)$|
|`rollback(t)`| タイムスタンプ $t$ のときに戻す． |最悪`unite`をした回数|
