---
title: Union-Find
documentation_of: ../../src/DataStructure/UnionFind.hpp
---

## `UnionFind` クラス
いわゆる普通の Union-Find．\
以下，$\alpha(n)$ はアッカーマン関数の逆関数であり，計算量はならしである 

|メンバ関数|概要|計算量|
|---|---|---|
|`UnionFind(n)`|コンストラクタ. 要素数 $n$ を渡す.||
|`size(u)`|要素 $u$ の属する集合のサイズを返す.||
|`leader(u)`|要素 $u$ の属する集合の代表元を返す.|$O(\alpha(n))$|
|`connected(u,v)`|要素 $u,v$ が同じ集合に属していれば `true` を返す．|$O(\alpha(n))$|
|`unite(u,v)`|要素 $u,v$ それぞれが属する集合を併合する．<br> すでに要素 $u,v$ が同じ集合に属していれば，`false` を返す． |$O(\alpha(n))$|
