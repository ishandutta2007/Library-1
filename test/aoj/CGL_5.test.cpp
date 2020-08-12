#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/5"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/_geometry_temp.hpp"
#include "src/Geometry/closest_pair.hpp"
#undef call_from_test

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