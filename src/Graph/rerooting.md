---
title: 全方位木DP
documentation_of: ./rerooting.hpp
---
## `rerooting` の戻り値 ( RerootingData クラス )
全方位木DPの値(型 : `T` )が入っている配列だと思って使う. (`operator[](int v)`, `begin()`, `end()` がある. )\
`get(int root, int v)` :  頂点 root が根である場合の 頂点 v を根とする部分木のDP値

## `rerooting` の引数
`rerooting<T,U,C,F1,F2,F3>(Tree<C> tree, F1 f_ee, F2 f_ve, F3 f_ev, U unit)` について \
`tree` : コスト `C` の木クラス \
`f_ee(U l, U r) -> U` : モノイド `U` の二項演算 \
`f_ve(T d, int v, Edge e) -> U` : 頂点 e.to のDPの値(型 : `T` ) から 辺 e の情報を用いて モノイドの値 (型 : `U` )に変換 \
`f_ev(U u, int v) -> T` : モノイドの値 (型 : `U` )から頂点 v のDPの値(型 : `T` ) に変換 \
`unit` : モノイド `U` の単位元

## 問題例
[AtCoder Regular Contest 022 C - ロミオとジュリエット](https://atcoder.jp/contests/arc022/tasks/arc022_3) \
[AtCoder Regular Contest 028 C - 高橋王国の分割統治](https://atcoder.jp/contests/arc028/tasks/arc028_3) \
[Educational DP Contest V - Subtree](https://atcoder.jp/contests/dp/tasks/dp_v) \
[Typical DP Contest N - 木](https://atcoder.jp/contests/tdpc/tasks/tdpc_tree) \
[square869120Contest #4 D - Driving on a Tree](https://atcoder.jp/contests/s8pc-4/tasks/s8pc_4_d)\
[NJPC2017 E - 限界集落](https://atcoder.jp/contests/njpc2017/tasks/njpc2017_e)\
[第二回全国統一プログラミング王決定戦本戦 D - 木、](https://atcoder.jp/contests/nikkei2019-2-final/tasks/nikkei2019_2_final_d) (根付き木ハッシュ)