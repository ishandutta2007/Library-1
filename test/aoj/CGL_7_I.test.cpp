#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_I"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/_geometry_temp.hpp"
#include "src/Geometry/intersection_area.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  Circle c, d;
  cin >> c.o >> c.r;
  cin >> d.o >> d.r;
  cout << intersection_area(c, d) << endl;
  return 0;
}