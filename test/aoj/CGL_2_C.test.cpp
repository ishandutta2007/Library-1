#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/2/CGL_2_C"

#define ERROR "0.00000001"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/!geometry_temp.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  cout << fixed << setprecision(12);
  int q;
  cin >> q;
  while (q--) {
    Segment s, t;
    cin >> s.p1 >> s.p2 >> t.p1 >> t.p2;
    cout << cross_points(s, t)[0] << endl;
  }
  return 0;
}