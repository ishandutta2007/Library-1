// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc256/tasks/abc256_Ex
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/abc256/tasks/abc256_h
// beats
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/DataStructure/SegmentTree.hpp"
using namespace std;
struct Mono {
 struct T {
  long long sum, max;
 };
 static T ti() { return T(); }
 static T op(const T &vl, const T &vr) { return {vl.sum + vr.sum, max(vl.max, vr.max)}; }
 struct E {
  long long upd, div;
 };
 static bool mp(T &v, const E &f, int sz) {
  if (f.div == 0) return v.sum= (v.max= f.upd) * sz, true;
  if (v.sum != v.max * sz) return false;
  return v.sum= (v.max/= f.div) * sz, true;
 }
 static void cp(E &pre, const E &suf) {
  if (suf.div == 0) pre= suf;
  else pre.upd/= suf.div, pre.div= min(pre.div * suf.div, 1ll << 30);
 }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, Q;
 cin >> N >> Q;
 SegmentTree<Mono> seg(N);
 for (int i= 0; i < N; i++) {
  long long a;
  cin >> a;
  seg.unsafe_set(i, {a, a});
 }
 seg.build();
 while (Q--) {
  int q, L, R;
  cin >> q >> L >> R, L--;
  if (q == 3) cout << seg.prod(L, R).sum << '\n';
  else {
   long long x;
   cin >> x;
   if (q == 1) seg.apply(L, R, {0, x});
   if (q == 2) seg.apply(L, R, {x, 0});
  }
 }
 return 0;
}