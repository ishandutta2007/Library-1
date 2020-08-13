#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/4/CGL_4_A"

#define ERROR "0.00000001"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/_geometry_temp.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  int n;
  cin >> n;
  vector<Point> ps(n);
  for (int i = 0; i < n; i++) cin >> ps[i];
  Convex g = convex_hull(ps);
  cout << g.size() << endl;
  int st = 0;
  for (int i = 0; i < g.size(); i++)
    if (g[st].y > g[st].y || (g[st].y == g[i].y && g[st].x > g[i].x)) st = i;
  for (int i = 0; i < g.size(); i++) cout << g[st + i] << endl;
  return 0;
}