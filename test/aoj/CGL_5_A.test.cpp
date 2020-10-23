#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/5/CGL_5_A"
#define ERROR "0.00000001"
#include <bits/stdc++.h>
#include "src/Geometry/!geometry_temp.hpp"
#include "src/Geometry/closest_pair.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  int n;
  cin >> n;
  vector<Point> ps(n);
  for (int i = 0; i < n; i++) cin >> ps[i];
  auto pp = closest_pair(ps);
  cout << fixed << setprecision(12) << dist(pp.first, pp.second) << endl;
  return 0;
}