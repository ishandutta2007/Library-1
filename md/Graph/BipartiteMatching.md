---
title: 二部グラフのマッチング
documentation_of: ../../src/Graph/BipartiteMatching.hpp
---
明示的に二部グラフを表現する \
頂点の添字は左右それぞれ [0,L), [0,R) で表現
## メンバ関数

| 関数名                          | 内容                                                                  | 計算量                              |
| ------------------------------- | --------------------------------------------------------------------- | ----------------------------------- |
| `BipartiteMatching(L,R)`        | コンストラクタ. 二部グラフの左右のサイズ L, Rを渡す                   |                                     |
| `add_edge(l,r)`                 | 辺 (l,r) を追加                                                       | $\mathcal{O}(1)$                    |
| `erase_edge(l,r)`               | 辺 (l,r) を削除                                                       | $\mathcal{O}(E)$                    |
| `build()`                       | 二部グラフのマッチングを実行                                          | $\mathcal{O}\left(E\sqrt{V}\right)$ |
| `build<true>()`                 | 辞書順最小になるように二部グラフのマッチングを実行                    | $\mathcal{O}\left(EV\right)$        |
| `max_matching()`                | マッチングに使う辺を返す                                              |                                     |
| `l_to_r(l)`                     | 左側の頂点 l のマッチング相手(右側の頂点)を返す. いないなら -1        |                                     |
| `r_to_l(r)`                     | 右側の頂点 r のマッチング相手(左側の頂点)を返す. いないなら -1        |
| `used_as_vertex_cover_left(l)`  | (最小頂点被覆を一つ構成して) 左側の頂点 l を点被覆に使用するかの bool |                                     |
| `used_as_vertex_cover_right(r)` | (最小頂点被覆を一つ構成して) 右側の頂点 r を点被覆に使用するかの bool |                                     |


## 問題例
[Chokudai SpeedRun 002 K - 種類数 β](https://atcoder.jp/contests/chokudai_S002/tasks/chokudai_S002_k) (頂点:2e5+4e5, 辺:4e5) \
[HackerRank Drawing Rectangles](https://www.hackerrank.com/contests/university-codesprint-4/challenges/drawing-rectangles) (最小頂点被覆, 頂点:3e5+3e5, 辺:3e5)