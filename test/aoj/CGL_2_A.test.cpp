#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/2/CGL_2_A"
#include <bits/stdc++.h>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  int q;
  cin >> q;
  while (q--) {
    Line l, m;
    cin >> l.p1 >> l.p2 >> m.p1 >> m.p2;
    if (is_parallel(l, m))
      cout << 2 << endl;
    else if (is_orthogonal(l, m))
      cout << 1 << endl;
    else
      cout << 0 << endl;
  }
  return 0;
}