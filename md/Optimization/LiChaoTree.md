---
title: Li-Chao-Tree
documentation_of: ../../src/Optimization/LiChaoTree.hpp
---
( 例えば 一次関数などの ) 高々一点で交わる $x$ の関数群 $f_i(x) = f(x; \boldsymbol{p}^{(i)})= f(x;p^{(i)}_0,\dots,p^{(i)}_n)$ を扱うデータ構造. \
浮動小数点数もいけるはず. \
2点以上で交わる可能性がある関数群を挿入した時の動作は未定義.

## 使用例
```c++
auto f = [](long long x, int a, int b){return a * x + b;}; // ax+b
LiChaoTree lct(f);
auto tree = lct.make_tree<MINIMIZE>();
tree.insert(2, 3); // 2x+3
tree.insert(0, 2, 1, 1); // x+1 (0<= x < 2)
auto [val1, id1] = tree.query(1);
cout << val1 << " "<< id1 << endl; // 2 1
auto [val2, id2] = tree.query(-1);
cout << val2 << " "<< id2 << endl; // 1 0
```

## `LiChaoTree` クラス
まず初期化のために触るクラス. \
内部で関数やパラメータを格納したりの役割がある.

|メンバ関数|概要|
|---|---|
|`LiChaoTree(f, LB, UB)`|コンストラクタ. <br> $x$ の関数 $f(x;p_0,\dots,p_n)$ を渡す. <br> クエリとして考える $x$ の半開区間 $\lbrack \mathrm{LB}, \mathrm{UB})$ を渡す. ( デフォルトは `LB=-2e9`, `UB=2e9`)|
|`make_tree<sgn, persistent>()`| `LiChaoTreeInterface` クラスのオブジェクトを返す. <br> 何も関数が挿入されていない空のデータ構造を返す. <br> template 第一引数で最小か最大を指定する. (デフォルトは最小) <br> template 第二引数が true なら永続化. (デフォルトはfalse)|

## `LiChaoTreeInterface` クラス

メインで触ることになるもの.

|メンバ関数|概要|計算量|
|---|---|---|
|1. `insert(p_0,...,p_n)` <br> 2. `insert(l,r,p_0,...,p_n)`|1. 関数 $f(x;p_0,\dots,p_n)$ を挿入. <br> 2. 半開区間制約付きの関数 $f(x;p_0,\dots,p_n) \hspace{1mm}x\in\lbrack l,r) $ を挿入. | 1. $\mathcal{O}(\log n)$ <br> 2.$\mathcal{O}((\log n)^2)$　|
|`query(x)` | { $x$ における最小値(最大値), それを達成する関数の番号 } を返す. <br>存在しない場合 { 十分大きな(小さな)値, -1 } を返す. |$\mathcal{O}(\log n)$ | 

## 問題例

[Kyoto University Programming Contest 2019 L - タケノコ](https://atcoder.jp/contests/kupc2019/tasks/kupc2019_l) (永続) \
[AtCoder Regular Contest 051 D - 長方形](https://atcoder.jp/contests/arc051/tasks/arc051_d) (doubleでac) \
[Yandex Contest Алгоритм 2022 K. Stepwise subsequence ](https://contest.yandex.com/contest/42710/problems/K) (有理数クラス, doubleでac) \
[COLOCON -Colopl programming contest 2018- Final C - スペースエクスプローラー高橋君](https://atcoder.jp/contests/colopl2018-final/tasks/colopl2018_final_c) \
[技術室奥プログラミングコンテスト#2 F - NPCの家 (NPC's House)](https://atcoder.jp/contests/tkppc2/tasks/tkppc2016_f) 