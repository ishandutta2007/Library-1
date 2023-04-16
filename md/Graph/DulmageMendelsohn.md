---
title: Dulmage-Mendelsohn 分解
documentation_of: ../../src/Graph/DulmageMendelsohn.hpp
---
明示的に二部グラフを表現する \
頂点の添字は左右それぞれ [0,L), [0,R) で表現 \
K+1 個 に分割された集合(0~K)が得られたとして
- 左側の頂点： 0~K-1番目のいずれかの集合に属する $\iff$ マッチングに必ず使う
- 右側の頂点： 1~K番目のいずれかの集合に属する $\iff$ マッチングに必ず使う
- 左側の頂点lがa番目の集合, 右側の頂点rがb番目の集合に属する: 辺(l,r)が存在 $\implies$ a $\le$ b

## メンバ関数

| 関数名                   | 内容                                                | 計算量                              |
| ------------------------ | --------------------------------------------------- | ----------------------------------- |
| `DulmageMendelsohn(L,R)` | コンストラクタ. 二部グラフの左右のサイズ L, Rを渡す |                                     |
| `add_edge(l,r)`          | 辺 (l,r) を追加                                     |                                     |
| `build()`                | DM分解を実行                                        | $\mathcal{O}\left(E\sqrt{V}\right)$ |
| `component_num()`        | 分割された集合の個数(K+1)を返す                     |                                     |
| `left_belong(l)`         | 左側の頂点 l の所属先を返す                         |                                     |
| `right_belong(r)`        | 右側の頂点 r の所属先を返す                         |                                     |
| `left_block(k)`          | 左側の k 番目の集合を返す                           |                                     |
| `right_block(k)`         | 右側の k 番目の集合を返す                           |                                     |

## 参考
[https://en.wikipedia.org/wiki/Dulmage%E2%80%93Mendelsohn_decomposition](https://en.wikipedia.org/wiki/Dulmage%E2%80%93Mendelsohn_decomposition) \
[http://www.misojiro.t.u-tokyo.ac.jp/~murota/lect-ouyousurigaku/dm050410.pdf](http://www.misojiro.t.u-tokyo.ac.jp/~murota/lect-ouyousurigaku/dm050410.pdf) \
[https://hitonanode.github.io/cplib-cpp/graph/dulmage_mendelsohn_decomposition.hpp.html](https://hitonanode.github.io/cplib-cpp/graph/dulmage_mendelsohn_decomposition.hpp.html)