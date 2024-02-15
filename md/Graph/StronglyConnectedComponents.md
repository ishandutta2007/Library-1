---
title: 強連結成分分解
documentation_of: ../../src/Graph/StronglyConnectedComponents.hpp
---
強連結成分はトポロジカルソートされている

## メンバ関数

| メンバ関数                           | 概要                                      |
| -------------------------------- | ------------------------------------------- |
| `StronglyConnectedComponents(g)` | コンストラクタ. 引数は [`Graph` クラス](Graph.hpp) |
| `size()`               | 強連結成分の個数を返す                      |
| `block(k)`                       | k 番目の強連結成分の頂点集合を返す          |
| `operator(i)`                      | 頂点 i が属する強連結成分が何番目かを返す   |
| `dag()`                          | 強連結成分を一つの頂点に潰したDAGを返す                           |
