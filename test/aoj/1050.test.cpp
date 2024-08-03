// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/1050
// competitive-verifier: ERROR 0.00000001
#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include "src/Geometry/SegmentArrangement.hpp"
#include "src/Geometry/Convex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= long double;
 static constexpr R INF= 1e18;
 cout << fixed << setprecision(12);
 for (int n; cin >> n && n;) {
  vector<Point<R>> ps(n);
  for (int i= 0; i < n; ++i) cin >> ps[i];
  Convex<R> cv({Point<R>(0, 0), Point<R>(4, 0), Point<R>(4, 4), Point<R>(0, 4)});
  vector<Segment<R>> ss;
  for (int i= n; i--;) {
   auto c= cv;
   for (int j= n; j--;) {
    if (j == i) continue;
    c= c.cut(bisector(ps[i], ps[j]));
   }
   for (auto e: c.edges())
    if (!cv.out(e)) ss.push_back(e);
  }
  SegmentArrangement<R> sa(ss);
  int m= sa.vertex_size();
  vector<R> dis(m, INF);
  priority_queue<pair<R, int>> pq;
  for (int i= m; i--;) {
   auto p= sa.point(i);
   if (sgn(p.x)) continue;
   dis[i]= 0, pq.emplace(0, i);
  }
  while (!pq.empty()) {
   auto [d, i]= pq.top();
   pq.pop(), d= -d;
   if (sgn(dis[i] - d)) continue;
   for (auto e: sa.out_edges(i)) {
    auto j= sa.to_v(e);
    auto nd= d + dist(sa.point(i), sa.point(j));
    if (sgn(dis[j] - nd) > 0) dis[j]= nd, pq.emplace(-nd, j);
   }
  }
  R ans= INF;
  for (int i= m; i--;) {
   auto p= sa.point(i);
   if (sgn(p.x - 4)) continue;
   ans= min(ans, dis[i]);
  }
  if (ans == INF) cout << "impossible" << '\n';
  else cout << ans << '\n';
 }
 return 0;
}