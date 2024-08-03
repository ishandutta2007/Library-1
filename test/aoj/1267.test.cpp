// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/1267
#include <iostream>
#include <vector>
#include "src/Geometry/Convex.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= long double;
 R mx= 10000;
 Convex<R> all({{0, 0}, {mx, 0}, {mx, mx}, {0, mx}});
 for (int n; cin >> n && n;) {
  vector<Point<R>> ps(n);
  for (int i= 0; i < n; ++i) cin >> ps[i];
  auto cv= all;
  for (int i= 0; i < n; ++i) cv= cv.cut(line_through(ps[i], ps[(i + 1) % n]));
  cout << (cv.size() > 0) << '\n';
 }
 return 0;
}