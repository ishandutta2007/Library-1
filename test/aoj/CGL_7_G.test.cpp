#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_G"
#define ERROR "0.00000001"
#include <bits/stdc++.h>
#include "src/Geometry/!geometry_temp.hpp"
#include "src/Geometry/circle_functions.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  cout << fixed << setprecision(12);
  Circle c, d;
  cin >> c.o >> c.r;
  cin >> d.o >> d.r;
  auto ls = common_tangent(c, d);
  vector<Point> ps;
  for (Line l : ls) ps.push_back(l.p1);
  sort(ps.begin(), ps.end());
  for (Point p : ps) cout << p << endl;
  return 0;
}