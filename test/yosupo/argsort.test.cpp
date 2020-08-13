#define PROBLEM "https://judge.yosupo.jp/problem/sort_points_by_argument"

#define ERROR "0.00000001"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/_geometry_temp.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using namespace geometry;
  int N;
  cin >> N;
  vector<Point> ps(N);
  for (int i = 0; i < N; i++) cin >> ps[i];
  sort(ps.begin(), ps.end(), polar_angle());
  for (Point p : ps) cout << p << endl;
  return 0;
}