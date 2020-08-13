#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/1/CGL_1_A"

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
  Line l;
  cin >> l.p1 >> l.p2;
  int q;
  cin >> q;
  while (q--) {
    Point p;
    cin >> p;
    cout << l.project(p) << endl;
  }
  return 0;
}