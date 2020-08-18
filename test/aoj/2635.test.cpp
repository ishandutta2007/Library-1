#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2635"

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
  vector<Point> ps(n);
  for (int i = 0; i < n; i++) cin >> ps[i];
  bool isok = true;
  vector<Point> pps;
  Real pre = 0;
  auto check = [&](Point p) {
    pps.push_back(p);
    if (pps.size() >= 3) {
      Convex ch = convex_hull(pps);
      Real cur = ch.area();
      bool res = sgn(cur - pre) > 0;
      pre = cur;
      return res;
    }
    return true;
  };
  for (int i = 0; i < n; i++) isok &= check(ps[i]);
  pps.clear();
  pre = 0;
  for (int i = n - 1; i >= 0; i--) isok &= check(ps[i]);
  cout << (isok ? "Possible" : "Impossible") << endl;
  return 0;
}