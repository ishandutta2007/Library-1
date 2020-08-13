#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/4/CGL_4_B"

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
  int n;
  cin >> n;
  Convex g(n);
  for (int i = 0; i < n; i++) cin >> g[i];
  cout << fixed << setprecision(12) << g.diameter() << endl;
  return 0;
}