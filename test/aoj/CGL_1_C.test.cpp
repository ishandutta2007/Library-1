#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/1/CGL_1_C"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/!geometry_temp.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  Point p0, p1;
  cin >> p0 >> p1;
  int q;
  cin >> q;
  while (q--) {
    Point p2;
    cin >> p2;
    switch (ccw(p0, p1, p2)) {
      case COUNTER_CLOCKWISE:
        cout << "COUNTER_CLOCKWISE" << endl;
        break;
      case CLOCKWISE:
        cout << "CLOCKWISE" << endl;
        break;
      case ONLINE_BACK:
        cout << "ONLINE_BACK" << endl;
        break;
      case ONLINE_FRONT:
        cout << "ONLINE_FRONT" << endl;
        break;
      case ON_SEGMENT:
        cout << "ON_SEGMENT" << endl;
        break;
    }
  }
  return 0;
}