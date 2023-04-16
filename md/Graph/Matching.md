---
title: 最大マッチング
documentation_of: ../../src/Graph/Matching.hpp
---
GabowのEdmonds' Algorithm
## メンバ関数

| 関数名           | 内容                                             | 計算量                                  |
| ---------------- | ------------------------------------------------ | --------------------------------------- |
| `Matching(N)`    | コンストラクタ. グラフの頂点数 N を渡す          |                                         |
| `add_edge(u,v)`  | 辺 (u,v) を追加                                  |                                         |
| `build()`        | マッチングを実行                                 | $\mathcal{O}(VE \log V)$ 思ったより速い |
| `match(v)`       | 頂点 v のマッチング相手を返す. 存在しないなら -1 |                                         |
| `max_matching()` | マッチングに使う辺を返す                         |                                         |


## 問題例
[Chokudai SpeedRun 002 K - 種類数 β](https://atcoder.jp/contests/chokudai_S002/tasks/chokudai_S002_k) (二部マッチング, 頂点:2e5+4e5, 辺:4e5) 