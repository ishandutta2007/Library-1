---
title: Segment-Tree
documentation_of: ../../src/DataStructure/SegmentTree.hpp
---
非2冪 <br>
mp関数では引数としてsizeを渡すこともできる

- `void mp(T&,E)` 
- `void mp(T&,E,int)` : sizeを渡す
- `bool mp(T&,E)` : beats!
- `bool mp(T&,E,int)` : beats! sizeを渡す

min_left, max_right は [ACL](https://github.com/atcoder/ac-library/blob/master/document_ja/segtree.md) と同じ挙動

## 計算量
$O(\log N)$
## 問題例
[AtCoder Regular Contest 027 D - ぴょんぴょんトレーニング](https://atcoder.jp/contests/arc027/tasks/arc027_4) (メモリギリギリ) 
