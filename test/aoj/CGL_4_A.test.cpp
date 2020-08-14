#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/4/CGL_4_A"

#define ERROR "0.00000001"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/!geometry_temp.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  cout << fixed << setprecision(12);
  int n;
  cin >> n;
  vector<Point> ps(n);
  for (int i = 0; i < n; i++) cin >> ps[i];
  Convex g = convex_hull(ps);
  cout << g.size() << endl;
  int st = 0;
  for (int i = 0; i < (int)g.size(); i++)
    if (g[st].y > g[i].y || (g[st].y == g[i].y && g[st].x > g[i].x)) st = i;
  for (int i = 0, j = st; i < (int)g.size(); i++, j = g.next(j))
    cout << g[j] << endl;
  return 0;
}