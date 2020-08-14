#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/3/CGL_3_C"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/!geometry_temp.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  int n;
  cin >> n;
  Polygon g(n);
  for (int i = 0; i < n; i++) cin >> g[i];
  int q;
  cin >> q;
  while (q--) {
    Point p;
    cin >> p;
    cout << g.contains(p) << endl;
  }
  return 0;
}