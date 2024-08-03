// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2423
// 辞書順マッチングのverify
#include <iostream>
#include <vector>
#include "src/Graph/BipartiteGraph.hpp"
#include "src/Geometry/Circle.hpp"
#include "src/Geometry/min_enclosing_circle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= long double;
 int n, m;
 cin >> n >> m;
 BipartiteGraph bg(m, n);
 R rs[n];
 for (int i= 0; i < n; i++) cin >> rs[i];
 for (int j= 0; j < m; j++) {
  int p;
  cin >> p;
  vector<Point<R>> ps(p);
  for (int k= 0; k < p; k++) cin >> ps[k];
  R r= min_enclosing_circle(ps).r;
  for (int i= 0; i < n; i++)
   if (sgn(rs[i] - r) >= 0) bg.add_edge(j, i + m);
 }
 auto [mc, mate]= bipartite_matching<true>(bg);
 if (mc.size() < m) cout << "NG" << '\n';
 else
  for (int l= 0; l < m; ++l) cout << mate[l] - m + 1 << '\n';
 return 0;
}