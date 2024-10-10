// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/range_affine_range_sum
// competitive-verifier: TLE 2
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <array>
#include "src/DataStructure/SegmentTree.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
using Mint= ModInt<998244353>;
// RsumQはモノイドでサイズを持っておく
struct RaffineQ_RsumQ {
 using T= Mint;
 using E= array<Mint, 2>;
 static T ti() { return 0; }
 static T op(const T &l, const T &r) { return l + r; }
 static bool mp(T &v, const E &f, int sz) { return v= f[0] * v + f[1] * sz, true; }
 static void cp(E &pre, const E &suf) { pre[0]*= suf[0], pre[1]= suf[0] * pre[1] + suf[1]; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 vector<Mint> v(N);
 for (int i= 0; i < N; i++) cin >> v[i];
 SegmentTree<RaffineQ_RsumQ> seg(v);
 while (Q--) {
  bool op;
  int l, r;
  cin >> op >> l >> r;
  if (op) {
   cout << seg.prod(l, r) << '\n';
  } else {
   Mint b, c;
   cin >> b >> c;
   seg.apply(l, r, {b, c});
  }
 }
 return 0;
}
