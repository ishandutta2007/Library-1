#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_I"

#define ERROR "0.000001"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/!geometry_temp.hpp"
#include "src/Geometry/circle_functions.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  Circle c, d;
  cin >> c.o >> c.r;
  cin >> d.o >> d.r;
  cout << fixed << setprecision(12) << intersection_area(c, d) << endl;
  return 0;
}