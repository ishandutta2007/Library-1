#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_H"

#define ERROR "0.00000001"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/!geometry_temp.hpp"
#include "src/Geometry/intersection_area.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  Circle c;
  int n;
  cin >> n >> c.r;
  c.o = {0, 0};
  Polygon g(n);
  for (int i = 0; i < n; i++) cin >> g[i];
  cout << fixed << setprecision(12) << intersection_area(c, g) << endl;
  return 0;
}