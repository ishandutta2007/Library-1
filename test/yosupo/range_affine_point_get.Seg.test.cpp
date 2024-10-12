// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/range_affine_point_get
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// 双対 の verify

#include <iostream>
#include <array>
#include "src/Math/ModInt.hpp"
#include "src/DataStructure/SegmentTree.hpp"
using namespace std;
using Mint= ModInt<998244353>;
struct M {
 using T= Mint;
 using E= array<Mint, 2>;
 static void mp(T &v, E x) { v= x[0] * v + x[1]; }
 static void cp(E &x, E y) { x= {y[0] * x[0], y[0] * x[1] + y[1]}; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, Q;
 cin >> N >> Q;
 vector<Mint> a(N);
 for (int i= 0; i < N; i++) cin >> a[i];
 SegmentTree<M> seg(a);
 while (Q--) {
  int t;
  cin >> t;
  if (t) {
   int i;
   cin >> i;
   cout << seg[i] << '\n';
  } else {
   int l, r;
   Mint b, c;
   cin >> l >> r >> b >> c;
   seg.apply(l, r, {b, c});
  }
 }
 return 0;
}