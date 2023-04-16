---
title: 強連結成分分解
documentation_of: ../../src/Graph/StronglyConnectedComponents.hpp
---
強連結成分はトポロジカルソートされている

## メンバ関数

| 関数名                           | 内容                                        |
| -------------------------------- | ------------------------------------------- |
| `StronglyConnectedComponents(N)` | コンストラクタ. 有向グラフの頂点数 N を渡す |
| `add_edge(s,d)`                  | 有向辺 (s,d) を追加                         |
| `build()`                        | 強連結成分分解を実行                        |
| `components_num()`               | 強連結成分の個数を返す                      |
| `block(k)`                       | k 番目の強連結成分の頂点集合を返す          |
| `belong(i)`                      | 頂点 i が属する強連結成分が何番目かを返す   |
| `dag()`                          | 縮約したDAGをを返す                         |
