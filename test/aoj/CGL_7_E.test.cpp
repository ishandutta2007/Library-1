#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_E"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/_geometry_temp.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  Circle c, d;
  cin >> c.o >> c.r;
  cin >> d.o >> d.r;
  auto ps = cross_points(c, d);
  Point p1 = ps[0];
  Point p2 = ps.size() > 1 ? ps[1] : ps[0];
  if (p2 < p1) swap(p1, p2);
  cout << p1 << " " << p2 << endl;
  return 0;
}