// competitive-verifier: PROBLEM https://codeforces.com/gym/103373/problem/I
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include <vector>
#include <cmath>
#include "src/Graph/BipartiteGraph.hpp"
#include "src/Optimization/matroid_intersection.hpp"

using namespace std;
class TransversalMatroid {
 vector<vector<int>> l2r;
 std::vector<int8_t> use;
 int L, R;
public:
 TransversalMatroid(int L, int R): l2r(L), L(L), R(R) {}
 void add_edge(int l, int r) { l2r[l].push_back(r); }
 void build(const std::vector<int> &I) {
  use.assign(l2r.size(), 0);
  for (int i: I) use[i]= 1;
 }
 inline bool oracle(int e) {
  use[e]= 1;
  BipartiteGraph bg(L, R);
  int cnt= 0;
  for (int l= L; l--;)
   if (use[l]) {
    ++cnt;
    for (int r: l2r[l]) bg.add_edge(l, r + L);
   }
  use[e]= 0;
  auto [edge, _]= bipartite_matching(bg);
  return (int)edge.size() == cnt;
 }
 inline bool oracle(int e, int f) {
  use[e]= 0, use[f]= 1;
  BipartiteGraph bg(L, R);
  int cnt= 0;
  for (int l= L; l--;)
   if (use[l]) {
    ++cnt;
    for (int r: l2r[l]) bg.add_edge(l, r + L);
   }
  use[e]= 1, use[f]= 0;
  auto [edge, _]= bipartite_matching(bg);
  return (int)edge.size() == cnt;
 }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int n, m;
 cin >> n >> m;
 long long a[n];
 for (int i= 0; i < n; ++i) cin >> a[i];
 GraphicMatroid g(n);
 vector<long long> cost(m);
 for (int i= 0; i < m; ++i) {
  int u, v;
  cin >> u >> v, --u, --v;
  g.add_edge(u, v);
  cost[i]= sqrt(a[u] + a[v]);
 }
 int w;
 cin >> w;
 TransversalMatroid M(m, w);
 for (int i= 0; i < w; ++i) {
  int x;
  cin >> x;
  for (int j= 0; j < x; ++j) {
   int b;
   cin >> b, --b;
   M.add_edge(b, i);
  }
 }
 auto S= weighted_matroid_intersection<MAXIMIZE>(m, g, M, cost);
 int K= S.size();
 vector<long long> ans(n - 1, -1);
 for (int i= 1; i < K; ++i) {
  ans[i - 1]= 0;
  for (auto x: S[i]) ans[i - 1]+= cost[x];
 }
 for (int i= 0; i < n - 1; ++i) cout << (i ? "" : " ") << ans[i];
 cout << '\n';
 return 0;
}