---
title: 木クラス
documentation_of: ./Tree.hpp
---
HL分解＋オイラーツアーで頂点集合を数列に \
非連結(森)でも動くはず

## メンバ関数
重みなし ( `Cost=void` ) \
　`add_edge(int u, int v)` : 無向辺 (u, v) を追加 \
重みあり \
　`add_edge(int u, int v, Cost c)` : 無向辺 (u, v) を重み c で追加 \
　`add_edge(int u, int v, Cost c, Cost d)` : 有向辺 (u, v) を重み c で、 有向辺 (v, u) を重み d で追加

`build(int root=0)` : 頂点 root を根としてHL分解を実行. 辺をセットし終えたらとにかく最初に実行. \
`path(int u, int v)` : 頂点 u から頂点 v へのパスを表す"閉"区間列を返す. \
`subtree(int v)` : 頂点 v を根とする部分木を表す"開"区間を返す \
`jump(int u, int v, int k)` : 頂点 u から 頂点 v へ向けて 長さ k 移動した先の頂点 \
`operator[](int v)` : 頂点 v から隣接する辺集合