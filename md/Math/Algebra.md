---
title: 代数的構造
documentation_of: ../../src/Math/Algebra.hpp
---

群の演算を`operator+`や`operator-`にしてBITやポテンシャルUFに乗せたり，半環にして行列に乗せたりするために用意した．

```cpp
struct M{
  using T = int;
  static inline T o = 0;
  static inline T i = 1;
  static T add(T a,T b){return a+b;}
  static T neg(T a){return -a;}
  static T mul(T a,T b){return a*b;}
};
```

## Verify


- [AtCoder Beginner Contest 009 D - 漸化式](https://atcoder.jp/contests/abc009/tasks/abc009_4) (bitwise xor and 半環)<br>
- [DISCO presents ディスカバリーチャンネル コードコンテスト2020 本戦 B - Hawker on Graph](https://atcoder.jp/contests/ddcc2020-final/tasks/ddcc2020_final_b) (max+の一次関数の加法合成の半環)<br>
- [競プロ典型 90 問　068 - Paired Information（★5）](https://atcoder.jp/contests/typical90/tasks/typical90_bp) (affine, 非可換群)


