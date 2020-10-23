#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/3/CGL_3_C"
#include <bits/stdc++.h>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;

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
    switch (g.where(p)) {
      case IN:
        cout << 2 << endl;
        break;
      case ON:
        cout << 1 << endl;
        break;
      case OUT:
        cout << 0 << endl;
        break;
    }
  }
  return 0;
}