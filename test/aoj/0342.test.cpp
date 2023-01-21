#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Prelim/0342"
#define ERROR "0.00000001"
#include <iostream>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
#include "src/Geometry/!geometry_temp.hpp"
#include "src/DataStructure/UnionFind.hpp"

using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geometry;
 int V, R;
 cin >> V >> R;
 vector<Point> ps(V);
 map<pair<int, int>, int> id;
 for (int i= 0; i < V; i++) {
  int x, y;
  cin >> x >> y;
  id[{x, y}]= i;
  ps[i]= {Real(x), Real(y)};
 }
 UnionFind uf(V), uf2(V);
 Convex ch= convex_hull(ps);
 Real ans= 0;
 for (int i= 0; i < (int)ch.size(); i++) {
  Point p= ch[i], q= ch[ch.next(i)];
  uf.unite(id[{round(p.x), round(p.y)}], id[{round(q.x), round(q.y)}]);
  ans+= dist(p, q);
 }

 vector<int> a, b;
 vector<Real> w;
 for (int i= 0; i < R; ++i) {
  int s, t;
  cin >> s >> t;
  if (uf.same(--s, --t)) continue;
  a.push_back(uf.root(s)), b.push_back(uf.root(t)), w.push_back(dist(ps[s], ps[t]));
 }
 int n= a.size();
 int ord[n];
 iota(ord, ord + n, 0), sort(ord, ord + n, [&](int l, int r) { return w[l] < w[r]; });
 for (int i: ord)
  if (uf2.unite(a[i], b[i])) ans+= w[i];

 cout << fixed << setprecision(12) << ans << endl;
 return 0;
}