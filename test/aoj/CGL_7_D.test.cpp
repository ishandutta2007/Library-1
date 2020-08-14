#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_D"

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
  Circle c;
  cin >> c.o >> c.r;
  int q;
  cin >> q;
  while (q--) {
    Line l;
    cin >> l.p1 >> l.p2;
    auto ps = cross_points(l, c);
    Point p1 = ps[0];
    Point p2 = ps.size() > 1 ? ps[1] : ps[0];
    if (p2 < p1) swap(p1, p2);
    cout << p1 << " " << p2 << endl;
  }
  return 0;
}