---
title: Weight-Balanced-Tree
documentation_of: ../../src/DataStructure/WeightBalancedTree.hpp
---

永続化できる \
単位元は必要なし（遅延側も）\
各ノードが葉のサイズを保持しているのでmapping関数では引数としてsizeを渡せる 
## 計算量
$O(\log N)$
## Verify
<details>
<summary>ここをクリックすると開きます</summary>

- [ ] [CODE FESTIVAL 2014 エキシビション B - カッコつけ](https://atcoder.jp/contests/code-festival-2014-exhibition/tasks/code_festival_exhibition_b) (insert, erase, prod)
- [ ] [SoundHound Programming Contest 2018 Masters Tournament 本戦 (Open) E - Hash Swapping](https://atcoder.jp/contests/soundhound2018-summer-final-open/tasks/soundhound2018_summer_final_e) (split3, prod)
- [ ] [2012年 日本情報オリンピック春合宿 copypaste - コピー＆ペースト](https://atcoder.jp/contests/joisc2012/tasks/joisc2012_copypaste) (融合永続, split, split3)
- [ ] [AtCoder Regular Contest 030 D - グラフではない](https://atcoder.jp/contests/arc030/tasks/arc030_4) (融合永続, split3, apply, prod)
- [ ] [洛谷 P5055 【模板】可持久化文艺平衡树](https://www.luogu.com.cn/problem/P5055) (全永続, insert, erase, prod, reverse, 強制オンラインクエリ)
- [ ] [洛谷 P5350 序列](https://www.luogu.com.cn/problem/P5350) (融合永続, split3, apply, prod, reverse, ML厳しめ)
- [ ] [洛谷 P5586 [P5350] 序列 (加强版)](https://www.luogu.com.cn/problem/P5586) (融合永続, split3, apply, prod, reverse, 強制オンラインクエリ)

</details>