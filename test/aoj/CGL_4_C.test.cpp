#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/4/CGL_4_C"
#define ERROR "0.00000001"
#include <bits/stdc++.h>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  cout << fixed << setprecision(12);
  int n;
  cin >> n;
  Convex g(n);
  for (int i = 0; i < n; i++) cin >> g[i];
  int q;
  cin >> q;
  while (q--) {
    Line l;
    cin >> l.p1 >> l.p2;
    cout << g.cut(l).area() << endl;
  }
  return 0;
}