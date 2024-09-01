// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/1033
// competitive-verifier: ERROR 0.00000001
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>
#include "src/Geometry/SegmentArrangement.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 cout << fixed << setprecision(10);
 using R= long double;
 static constexpr R INF= 1e18;
 for (int n; cin >> n && n;) {
  vector<Segment<R>> ss(n);
  for (int i= 0; i < n; ++i) cin >> ss[i].p >> ss[i].q;
  SegmentArrangement sa(ss);
  Point<R> s, g;
  cin >> s >> g;
  int si= sa.vertex(s), gi= sa.vertex(g);
  int m= sa.vertex_size();
  R dis[m][m + 1];
  for (int i= m; i--;) fill_n(dis[i], m + 1, INF);
  priority_queue<tuple<R, int, int>> pq;
  pq.emplace(0, si, m);
  dis[si][m]= 0;
  R ans= -1;
  while (!pq.empty()) {
   auto [d, v, p]= pq.top();
   pq.pop();
   d= -d;
   if (sgn(dis[v][p] - d) != 0) continue;
   if (v == gi) {
    ans= d;
    break;
   }
   for (int e: sa.out_edges(v)) {
    int u= sa.to_v(e);
    R nd= d;
    if (p != m) nd+= abs(angle(sa.point(v) - sa.point(p), sa.point(u) - sa.point(v)));
    if (sgn(dis[u][v] - nd) > 0) dis[u][v]= nd, pq.emplace(-nd, u, v);
   }
  }
  if (ans < 0) cout << "-1" << '\n';
  else cout << radian_to_degree(ans) << '\n';
 }
 return 0;
}