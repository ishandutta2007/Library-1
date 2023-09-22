#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/1033"
#define ERROR "0.00000001"
#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>
#include "src/Misc/compress.hpp"
#include "src/Geometry/Segment.hpp"
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
  Point<R> s, g;
  cin >> s >> g;
  vector<Point<R>> ps{s, g};
  for (int i= n; i--;)
   for (int j= i; j--;)
    if (auto cp= cross_points(ss[i], ss[j]); cp.size()) ps.insert(ps.end(), cp.begin(), cp.end());
  compress(ps);
  int m= ps.size();
  vector<int> adj[m];
  for (int i= m; i--;)
   for (int j= i; j--;) {
    bool isok= false;
    for (const auto &t: ss) isok|= t.on(ps[i]) && t.on(ps[j]);
    if (isok) adj[i].push_back(j), adj[j].push_back(i);
   }
  int si, gi;
  for (int i= m; i--;) {
   if (ps[i] == s) si= i;
   if (ps[i] == g) gi= i;
  }
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
   for (int u: adj[v]) {
    R nd= d;
    if (p != m) nd+= abs(angle(ps[v] - ps[p], ps[u] - ps[v]));
    if (sgn(dis[u][v] - nd) > 0) dis[u][v]= nd, pq.emplace(-nd, u, v);
   }
  }
  if (ans < 0) cout << "-1" << '\n';
  else cout << radian_to_degree(ans) << '\n';
 }
 return 0;
}