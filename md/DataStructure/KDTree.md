---
title: kD-Tree
documentation_of: ../../src/DataStructure/KDTree.hpp
---

$O(k\cdot N^{1-1/k})$　で動くと思っておく.\
$Q=10^5$ は大丈夫だけど $Q=2\times 10^5$ だと無理なイメージ. \
領域を与える類のは全て境界は含まれるとする. (例えば矩形は閉区間の直積)\
半群は乗らない. モノイドの単位元 `M::ti` は明示的に与える必要あり. \
内部でサイズを持たせていないので, `M::mp` はsizeを渡せない(2引数).

## テンプレート  `KDTree<class pos_t, size_t K, class M=void>`

`pos_t` : 座標(一次元)を表現する型. 通常 int とか使う.\
`K` : 次元数.　通常 2 とか 3.\
`M` : いつものモノイドとかのやつ or int などを与える. 点に持たせる値に関するクラス. 何も持たせないなら void.

## メンバ関数

|名前|概要|
|---|---|
|`KDTree(P* p, n)` <br> `KDTree(vector<P>& p)` <br> `KDTree(set<P>& p)` <br>(クラス`P` は `tuple<pos_t,...,pos_t>` like)|コンストラクタ. <br> n個の点の座標を与える. <br> 点に値を乗せる場合, この時の初期値はデフォルトかモノイドの単位元.|

使用例
```c++
vector<array<int,2>> xy(N);
for(auto&[x,y]:xy) cin>>x>>y;
KDTree<int,2> kdt(xy);
```

|||
|---|---|
|`KDTree(P* p,int n, U v)` <br> `KDTree(vector<P>& p, U v)` <br> `KDTree(set<P>& p, U v)` <br>(クラス`P` は `tuple<pos_t,...,pos_t>` like)|コンストラクタ. <br> n個の点の座標と点に乗せる共通の初期値を与える.|

使用例
```c++
vector<array<int,2>> xy(N);
for(auto&[x,y]:xy) cin>>x>>y;
KDTree<int,2,int> kdt(xy,1);
```

|||
|---|---|
|`KDTree(P* p,int n)` <br> `KDTree(vector<P>& p)` <br> (クラス`P` は `tuple<pos_t,...,pos_t, T>` like)|コンストラクタ. <br> n個の点の座標と各々の点に乗せる初期値を与える.|

使用例
```c++
vector<array<int,3>> xyv(N);
for(auto&[x,y,v]:xyv) cin>>x>>y>>v;
KDTree<int,2,int> kdt(xyv);
```

|||
|---|---|
|`KDTree(pair<P,U>* p,int n)` <br> `KDTree(vector<pair<P,U>>& p)` <br> `KDTree(map<P,U>& p)` <br> (クラス`P` は `tuple<pos_t,...,pos_t>` like)|コンストラクタ. <br> n個の点の座標と各々の点に乗せる初期値を与える.|

使用例
```c++
map<array<int,2>,int> xyv;
for(int i=0;i<N;++i){
 int x,y,v;cin>>x>>y>>v;
 xyv[{x,y}] += v;
}
KDTree<int,2,int> kdt(xyv);
```

|||
|---|---|
|`enum_cuboid(x_1l,x_1r,...x_kl,x_kr)`| 直方体(長方形) 内部に位置する点についてその点に乗っている値が列挙される. <br> 引数は 2K 個 ( $\lbrack x^l_1,x^r_1\rbrack\times\cdots\times\lbrack x^l_K,x^r_K\rbrack$ ). |
|`enum_ball(x_1,...x_k,r)`| 球(円) 内部に位置する点についてその点に乗っている値が列挙される.<br> 引数は K+1 個 ( 中心: $(x_1,\dots,x_K)$, 半径: $r$ ). |
|`fold_cuboid(x_1l,x_1r,...x_kl,x_kr)`| 直方体(長方形) 内部に位置する点についてその点に乗っている値を集約した値を返す. <br> 引数は 2K 個 ( $\lbrack x^l_1,x^r_1\rbrack\times\cdots\times\lbrack x^l_K,x^r_K\rbrack$ ). |
|`fold_ball(x_1,...x_k,r)`| 球(円) 内部に位置する点についてその点に乗っている値を集約した値を返す.<br> 引数は K+1 個 ( 中心: $(x_1,\dots,x_K)$, 半径: $r$ ). |
|`apply_cuboid(x_1l,x_1r,...x_kl,x_kr, a)`| 直方体(長方形) 内部に位置する点についてその点に乗っている値に `a` を作用させる. <br> 引数は 2K+1 個 ( $\lbrack x^l_1,x^r_1\rbrack\times\cdots\times\lbrack x^l_K,x^r_K\rbrack$ と 作用素 `a` ). |
|`apply_ball(x_1,...x_k,r, a)`| 球(円) 内部に位置する点についてその点に乗っている値を集約した値に `a` を作用させる. <br> 引数は K+1+1 個 ( 中心: $(x_1,\dots,x_K)$, 半径: $r$ と 作用素 `a`). |
|`set(x_1,...x_k, v)`|点 $(x_1,\dots,x_K)$ の値を `v` に変更する. <br> 点が存在しないとassertで落ちる.|
|`get(x_1,...x_k)`|点 $(x_1,\dots,x_K)$ の値を返す.　<br> 点が存在しないとassertで落ちる.|
|`mul(x_1,...x_k, v)`|点 $(x_1,\dots,x_K)$ の値に `v` を (モノイド演算で) かける. <br> 点が存在しないとassertで落ちる.|
|`nearest_neighbor(x_1,...,x_k)`|点 $(x_1,\dots,x_K)$ と最も近い点を返す. <br> 点の座標を返す. <br> そもそも一つも点がない場合は assert で落ちる.|

## 参考
[https://trap.jp/post/1489/](https://trap.jp/post/1489/)
## 問題例
[square869120Contest #4 G - Get the Salary of Atcoder](https://atcoder.jp/contests/s8pc-4/tasks/s8pc_4_g) (遅延伝搬) \
[第二回 アルゴリズム実技検定 過去問 N - ビルの建設](https://atcoder.jp/contests/past202004-open/tasks/past202004_n) (双対) \
[AtCoder Beginner Contest 234 Ex - Enumerate Pairs](https://atcoder.jp/contests/abc234/tasks/abc234_h) (円形クエリ, 列挙) \
[AtCoder Beginner Contest 266 Ex - Snuke Panic (2D)](https://atcoder.jp/contests/abc266/tasks/abc266_h) (2次元 max) \
[AtCoder Regular Contest 010 D - 情報伝播](https://atcoder.jp/contests/arc010/tasks/arc010_4) (最近傍探索) \
[JOI 2018/2019 春合宿 過去問 A - 試験 (Examination)](https://atcoder.jp/contests/joisc2019/tasks/joisc2019_a)(3次元 sum ← 通る, or 2次元 sum + 走査 )\
[H - 3人の昼食 (The Lunch)](https://atcoder.jp/contests/s8pc-1/tasks/s8pc_1_h)(4 sec 強, TL には間に合う)\
[Happy Query Contest 2019 Grid Xor Query](https://www.hackerrank.com/contests/happy-query-contest/challenges/grid-xor-query) (2次元 xor)