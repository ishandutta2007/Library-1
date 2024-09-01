// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/JAG/Summer/2450
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
// Do use segment tree
#include <iostream>
#include <algorithm>
#include "src/DataStructure/LinkCutTree.hpp"
using namespace std;
struct RupdQ_RmaxsumQ {
 struct T {
  long long sum, max, lmax, rmax;
  int size;
 };
 using E= long long;
 static T op(const T &a, const T &b) {
  T ret;
  ret.sum= a.sum + b.sum;
  ret.max= max({a.max, b.max, a.rmax + b.lmax});
  ret.lmax= max(a.lmax, a.sum + b.lmax);
  ret.rmax= max(a.rmax + b.sum, b.rmax);
  ret.size= a.size + b.size;
  return ret;
 }
 static void mp(T &v, const E &f) {
  v.sum= f * v.size;
  v.max= v.lmax= v.rmax= max(v.sum, f);
 }
 static void cp(E &pre, const E &suf) { pre= suf; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 LinkCutTree<RupdQ_RmaxsumQ> lct(N);
 for (int i= 0; i < N; ++i) {
  long long w;
  cin >> w;
  lct.set(i, {w, w, w, w, 1});
 }
 for (int i= 0; i < N - 1; i++) {
  int u, v;
  cin >> u >> v;
  lct.link(--u, --v);
 }
 while (Q--) {
  long long t, a, b, c;
  cin >> t >> a >> b >> c, --a, --b;
  if (t == 1) lct.apply(a, b, c);
  else cout << lct.prod(a, b).max << '\n';
 }
 return 0;
}
