#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/2/CGL_2_B"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/_geometry_temp.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  Segment s, t;
  cin >> s.p1 >> s.p2 >> t.p1 >> t.p2;
  cout << (cross_points(s, t).size() > 0) << endl;
  return 0;
}