// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/880
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// beats!
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cstdint>
#include "src/DataStructure/SegmentTree.hpp"
using namespace std;
struct Mono {
 struct T {
  uint64_t max, sum, lcm;
 };
 static T ti() { return {0, 0, 1}; }
 static T op(const T &vl, const T &vr) { return {max(vl.max, vr.max), vl.sum + vr.sum, min(1ull * lcm(vl.lcm, vr.lcm), 1ull << 60)}; }
 struct E {
  uint32_t upd, gcd;
 };
 static bool mp(T &v, const E &f, int sz) {
  if (f.gcd == 0) return v.sum= (v.lcm= v.max= f.upd) * sz, true;
  if (f.gcd % v.lcm == 0) return true;
  if (v.max * sz != v.sum) return false;
  return v.sum= (v.lcm= v.max= gcd(v.max, f.gcd)) * sz, true;
 }
 static void cp(E &pre, const E &suf) {
  if (pre.gcd != 0 && suf.gcd != 0) pre.gcd= gcd(pre.gcd, suf.gcd);
  else pre= {suf.gcd == 0 ? suf.upd : gcd(pre.upd, suf.gcd), 0};
 }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, Q;
 cin >> N >> Q;
 SegmentTree<Mono> seg(N);
 for (int i= 0; i < N; i++) {
  unsigned a;
  cin >> a;
  seg.unsafe_set(i, {a, a, a});
 }
 seg.build();
 while (Q--) {
  int op, l, r;
  cin >> op >> l >> r, l--;
  if (op < 3) {
   unsigned x;
   cin >> x;
   if (op == 1) seg.apply(l, r, {x, 0});
   if (op == 2) seg.apply(l, r, {0, x});
  } else {
   if (op == 3) cout << seg.prod(l, r).max << '\n';
   if (op == 4) cout << seg.prod(l, r).sum << '\n';
  }
 }
 return 0;
}