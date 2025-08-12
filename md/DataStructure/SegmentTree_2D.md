---
title: Segment-Tree(2次元)
documentation_of: ../../src/DataStructure/SegmentTree_2D.hpp
---

半群は乗らない. モノイドの単位元 `M::ti` は明示的に与える必要あり. 

## テンプレート  `SegmentTree_2D<class pos_t, class M>`

`pos_t` : 座標(一次元)を表現する型. 通常 int とか使う.\
`M` : いつものモノイドのやつを与える. 

## メンバ関数

使用例で出てくる`RMQ`は次のような感じ.
```c++
struct RMQ{
 using T=int;
 static T ti(){return 1<<31;}
 static T op(T l,T r){return min(l,r);}
};
```

|名前|概要|
|---|---|
|`SegmentTree_2D(P* p, n)` <br> `SegmentTree_2D(vector<P>& p)` <br> `SegmentTree_2D(set<P>& p)` <br>(クラス`P` は `tuple<pos_t,pos_t>` like)|コンストラクタ. <br> n個の点の座標を与える. <br> この時の初期値はモノイドの単位元.|

使用例
```c++
vector<array<int>> xy(N);
for(auto&[x,y]:xy) cin>>x>>y;
SegmentTree_2D<int,RMQ> seg(xy);
```

|||
|---|---|
|`SegmentTree_2D(P* p,int n, U v)` <br> `SegmentTree_2D(vector<P>& p, U v)` <br> `SegmentTree_2D(set<P>& p, U v)` <br>(クラス`P` は `tuple<pos_t,pos_t>` like)|コンストラクタ. <br> n個の点の座標と点に乗せる共通の初期値を与える.|

使用例
```c++
vector<array<int>> xy(N);
for(auto&[x,y]:xy) cin>>x>>y;
SegmentTree_2D<int,RMQ> seg(xy,1);
```

|||
|---|---|
|`SegmentTree_2D(P* p,int n)` <br> `SegmentTree_2D(vector<P>& p)` <br> (クラス`P` は `tuple<pos_t,pos_t,T>` like)|コンストラクタ. <br> n個の点の座標と各々の点に乗せる初期値を与える.|

使用例
```c++
vector<array<int,3>> xyv(N);
for(auto&[x,y,v]:xyv) cin>>x>>y>>v;
SegmentTree_2D<int,RMQ> seg(xyv);
```

|||
|---|---|
|`SegmentTree_2D(pair<P,U>* p,int n)` <br> `SegmentTree_2D(vector<pair<P,U>>& p)` <br> `SegmentTree_2D(map<P,U>& p)` <br> (クラス`P` は `tuple<pos_t,pos_t>` like)|コンストラクタ. <br> n個の点の座標と各々の点に乗せる初期値を与える.|

使用例
```c++
map<array<int>,int> xyv;
for(int i=0;i<N;++i){
 int x,y,v;cin>>x>>y>>v;
 xyv[{x,y}] += v;
}
SegmentTree_2D<int,RMQ> seg(xyv);
```

|||計算量|
|---|---|---|
|`prod(l,r,u,d)`| 直方体(長方形) 内部に位置する点についてその点に乗っている値を集約した値を返す. <br> **※半開区間** $\lbrack l,r )\times \lbrack u,d)$ |以下, 点の個数を $n$ とする. <br> $O((\log n)^2)$|
|`set(x,y,v)`|点 $(x,y)$ の値を `v` に変更する. <br> 点が存在しないとassertで落ちる.|$O(\log n)$|
|`get(x,y)`|点 $(x,y)$ の値を返す.　<br> 点が存在しないとassertで落ちる.|$O(\log n)$|
|`mul(x,y,v)`|点 $(x,y)$ の値に `v` を (モノイド演算で) かける. <br> 点が存在しないとassertで落ちる.|$O(\log n)$|

## Verify
<details>
<summary>ここをクリックすると開きます</summary>

<input disabled="" type="checkbox"> [技術室奥プログラミングコンテスト#6 Day1 N - Jump and Walk](https://atcoder.jp/contests/tkppc6-1/tasks/tkppc6_1_n) (2次元 min, kdtだとTLE)\
<input disabled="" type="checkbox"> [JOI 2018/2019 春合宿 過去問 A - 試験 (Examination)](https://atcoder.jp/contests/joisc2019/tasks/joisc2019_a)(2次元 sum + 走査)

</details>