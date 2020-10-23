#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_E"
#define ERROR "0.00000001"
#include <bits/stdc++.h>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;

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
  cout << fixed << setprecision(12) << p1 << " " << p2 << endl;
  return 0;
}