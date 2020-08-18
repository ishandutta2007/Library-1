#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_B"

#define ERROR "0.00000001"

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
  Point A, B, C;
  cin >> A >> B >> C;
  Circle c = inscribed_circle(A, B, C);
  cout << fixed << setprecision(12) << c.o << " " << c.r << endl;
  return 0;
}