// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/JAG/Summer/2450
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
// Do use segment tree
#include <iostream>
#include <algorithm>
#include "src/Graph/Graph.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/DataStructure/SegmentTree.hpp"
using namespace std;
struct M {
 struct T {
  long long sum, max, lmax, rmax;
 };
 using E= long long;
 static inline long long INF= 1ll << 60;
 static T ti() { return {0, -INF, -INF, -INF}; }
 static T op(const T &a, const T &b) {
  T ret;
  ret.sum= a.sum + b.sum;
  ret.max= max({a.max, b.max, a.rmax + b.lmax});
  ret.lmax= max(a.lmax, a.sum + b.lmax);
  ret.rmax= max(a.rmax + b.sum, b.rmax);
  return ret;
 }
 static void mp(T &v, const E &f, int sz) {
  v.sum= f * sz;
  v.max= v.lmax= v.rmax= max(v.sum, f);
 }
 static void cp(E &pre, const E &suf) { pre= suf; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 vector<long long> w(N);
 for (int u= 0; u < N; ++u) cin >> w[u];
 Graph g(N, N - 1);
 for (int i= 0; i < N - 1; ++i) cin >> g[i], --g[i];
 HeavyLightDecomposition hld(g);
 vector<typename M::T> ar(N);
 for (int u= N; u--;) ar[hld.to_seq(u)]= {w[u], w[u], w[u], w[u]};
 SegmentTree<M> seg1(ar);
 reverse(ar.begin(), ar.end());
 SegmentTree<M> seg2(ar);
 while (Q--) {
  long long t, a, b, c;
  cin >> t >> a >> b >> c, --a, --b;
  if (t == 1) {
   for (auto [x, y]: hld.path(a, b))
    if (x < y) seg1.apply(x, y + 1, c), seg2.apply(N - y - 1, N - x, c);
    else seg1.apply(y, x + 1, c), seg2.apply(N - x - 1, N - y, c);
  } else {
   typename M::T ans= M::ti();
   for (auto [x, y]: hld.path(a, b)) ans= M::op(ans, x < y ? seg1.prod(x, y + 1) : seg2.prod(N - x - 1, N - y));
   cout << ans.max << '\n';
  }
 }
 return 0;
}