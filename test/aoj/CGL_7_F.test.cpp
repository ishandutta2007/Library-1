#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_F"

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
  Point p;
  cin >> p;
  Circle c;
  cin >> c.o >> c.r;
  auto ls = c.tangent(p);
  Point p1 = ls[0].p2, p2 = ls[1].p2;
  if (p2 < p1) swap(p1, p2);
  cout << fixed << setprecision(12) << p1 << " " << p2 << endl;
  return 0;
}