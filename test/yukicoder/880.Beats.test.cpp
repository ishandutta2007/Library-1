// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/880
#include <iostream>
#include <algorithm>
#include <numeric>
#include "src/DataStructure/SegmentTree_Beats.hpp"
using namespace std;
struct Mono {
 struct T {
  uint64_t max, sum, lcm;
  uint32_t sz;
 };
 static T ti() { return {0, 0, 1, 0}; }
 static T op(const T &vl, const T &vr) { return {max(vl.max, vr.max), vl.sum + vr.sum, min(1ull * lcm(vl.lcm, vr.lcm), 1ull << 60), vl.sz + vr.sz}; }
 struct E {
  uint32_t upd, gcd;
 };
 static bool mp(T &v, const E &f) {
  if (f.gcd == 0) return v.sum= (v.lcm= v.max= f.upd) * v.sz, true;
  if (f.gcd % v.lcm == 0) return true;
  if (v.max * v.sz != v.sum) return false;
  return v.sum= (v.lcm= v.max= gcd(v.max, f.gcd)) * v.sz, true;
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
 SegmentTree_Beats<Mono> seg(N);
 for (int i= 0; i < N; i++) {
  unsigned a;
  cin >> a;
  seg.unsafe_set(i, {a, a, a, 1});
 }
 seg.rebuild();
 while (Q--) {
  int op, l, r;
  cin >> op >> l >> r, l--;
  if (op < 3) {
   unsigned x;
   cin >> x;
   if (op == 1) seg.apply(l, r, {x, 0});
   if (op == 2) seg.apply(l, r, {0, x});
  } else {
   if (op == 3) cout << seg.fold(l, r).max << '\n';
   if (op == 4) cout << seg.fold(l, r).sum << '\n';
  }
 }
 return 0;
}