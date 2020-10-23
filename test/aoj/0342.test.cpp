#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Prelim/0342"
#define ERROR "0.00000001"
#include <bits/stdc++.h>
#include "src/Geometry/!geometry_temp.hpp"
#include "src/DataStructure/UnionFind.hpp"
#include "src/Graph/MinimumSpanningTree_Kruskal.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  int V, R;
  cin >> V >> R;
  vector<Point> ps(V);
  map<pair<int, int>, int> id;
  for (int i = 0; i < V; i++) {
    int x, y;
    cin >> x >> y;
    id[{x, y}] = i;
    ps[i] = {Real(x), Real(y)};
  }
  UnionFind uf(V);
  Convex ch = convex_hull(ps);
  Real ans = 0;
  for (int i = 0; i < (int)ch.size(); i++) {
    Point p = ch[i], q = ch[ch.next(i)];
    uf.unite(id[{round(p.x), round(p.y)}], id[{round(q.x), round(q.y)}]);
    ans += dist(p, q);
  }
  MinimumSpanningTree_Kruskal<Real> graph(V);
  for (int i = 0; i < R; i++) {
    int s, t;
    cin >> s >> t;
    s--, t--;
    if (uf.same(s, t)) continue;
    graph.add_edge(uf.root(s), uf.root(t), dist(ps[s], ps[t]));
  }
  ans += graph.get_MST().first;
  cout << fixed << setprecision(12) << ans << endl;
  return 0;
}