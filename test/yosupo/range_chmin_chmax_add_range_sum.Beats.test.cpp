#define PROBLEM "https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum"
#include <iostream>
#include "src/DataStructure/SegmentTree_Beats.hpp"
using namespace std;
struct Mono {
 static constexpr long long INF= 1ll << 62;
 struct T {
  long long sum, h, l, h2, l2;
  int sz, hc, lc;
 };
 struct E {  // clamp(x,lb,ub)+ad
  long long lb, ub, ad;
  static E add(long long x) { return {-INF, INF, x}; }
  static E chmin(long long x) { return {-INF, x, 0}; }
  static E chmax(long long x) { return {x, INF, 0}; }
 };
 static long long min2(long long a, long long a2, long long b, long long b2) { return a == b ? min(a2, b2) : a2 <= b ? a2 : b2 <= a ? b2 : max(a, b); }
 static long long max2(long long a, long long a2, long long b, long long b2) { return a == b ? max(a2, b2) : a2 >= b ? a2 : b2 >= a ? b2 : min(a, b); }
 static T ti() { return {0, -INF, INF, -INF, INF, 0, 0, 0}; }
 static T op(const T &vl, const T &vr) { return {vl.sum + vr.sum, max(vl.h, vr.h), min(vl.l, vr.l), max2(vl.h, vl.h2, vr.h, vr.h2), min2(vl.l, vl.l2, vr.l, vr.l2), vl.sz + vr.sz, vl.hc * (vl.h >= vr.h) + vr.hc * (vl.h <= vr.h), vl.lc * (vl.l <= vr.l) + vr.lc * (vl.l >= vr.l)}; }
 static bool mp(T &v, const E &f) {
  if (v.h <= f.lb) {
   v.sum= (v.h= v.l= f.lb + f.ad) * (v.hc= v.lc= v.sz);
   v.h2= -INF, v.l2= INF;
   return true;
  }
  if (v.l >= f.ub) {
   v.sum= (v.h= v.l= f.ub + f.ad) * (v.hc= v.lc= v.sz);
   v.h2= -INF, v.l2= INF;
   return true;
  }
  if (f.lb <= v.l && v.h <= f.ub) {
   v.sum+= f.ad * v.sz, v.h+= f.ad, v.l+= f.ad;
   v.h2+= f.ad, v.l2+= f.ad;
   return true;
  }
  if (v.h2 <= f.lb) {
   v.l= v.h2= f.lb + f.ad, v.lc= v.sz - v.hc;
   v.l2= v.h= min(f.ub, v.h) + f.ad;
   v.sum= v.h * v.hc + v.l * v.lc;
   return true;
  }
  if (v.l2 >= f.ub) {
   v.h= v.l2= f.ub + f.ad, v.hc= v.sz - v.lc;
   v.h2= v.l= max(f.lb, v.l) + f.ad;
   v.sum= v.h * v.hc + v.l * v.lc;
   return true;
  }
  return false;
 }
 static void cp(E &pre, const E &suf) {
  if (auto tmpl= suf.lb - pre.ad; pre.ub <= tmpl) pre.ub= pre.lb= tmpl;
  else if (auto tmpu= suf.ub - pre.ad; tmpu <= pre.lb) pre.ub= pre.lb= tmpu;
  else pre.lb= max(pre.lb, tmpl), pre.ub= min(pre.ub, tmpu);
  pre.ad+= suf.ad;
 }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, Q;
 cin >> N >> Q;
 SegmentTree_Beats<Mono> seg(N);
 for (int i= 0; i < N; i++) {
  long long a;
  cin >> a;
  seg.unsafe_set(i, {a, a, a, -Mono::INF, Mono::INF, 1, 1, 1});
 }
 seg.rebuild();
 while (Q--) {
  int op, l, r;
  cin >> op >> l >> r;
  if (op == 3) {
   cout << seg.fold(l, r).sum << '\n';
  } else {
   long long b;
   cin >> b;
   if (op == 0) seg.apply(l, r, Mono::E::chmin(b));
   if (op == 1) seg.apply(l, r, Mono::E::chmax(b));
   if (op == 2) seg.apply(l, r, Mono::E::add(b));
  }
 }
 return 0;
}