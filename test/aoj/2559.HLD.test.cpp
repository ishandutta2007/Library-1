// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2559
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include "src/DataStructure/UnionFind.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/DataStructure/SegmentTree_Dual.hpp"

using namespace std;
struct RchminQ {
 using T= int;
 using E= int;
 static void mp(T &v, const E &f) {
  if (v > f) v= f;
 }
 static void cp(E &pre, const E &suf) {
  if (pre > suf) pre= suf;
 }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int n, m;
 cin >> n >> m;
 UnionFind uf(n);
 Graph g(n, m);
 vector<int> w(m), use(m);
 for (int i= 0; i < m; ++i) cin >> g[i] >> w[i], --g[i];

 long long mst_cost= 0;
 HeavyLightDecomposition hld([&]() {
  Graph mst(n);
  int ord[m];
  iota(ord, ord + m, 0), sort(ord, ord + m, [&](int l, int r) { return w[l] < w[r]; });
  for (int i: ord)
   if (auto [u, v]= g[i]; uf.unite(u, v)) mst_cost+= w[i], mst.add_edge(g[i]), use[i]= 1;
  return mst;
 }());

 static constexpr int INF= 1 << 30;
 SegmentTree_Dual<RchminQ> seg(n, INF);

 for (int i= m; i--;) {
  auto &[u, v]= g[i];
  if (use[i]) {
   if (hld.in_subtree(u, v)) swap(u, v);
  } else
   for (auto [x, y]: hld.path(u, v, true)) x < y ? seg.apply(x, y + 1, w[i]) : seg.apply(y, x + 1, w[i]);
 }

 for (int i= 0; i < m; ++i) {
  if (use[i]) {
   int tmp= seg[hld.to_seq(g[i].second)];
   cout << (tmp == INF ? -1 : mst_cost - w[i] + tmp) << '\n';
  } else cout << mst_cost << '\n';
 }
 return 0;
}