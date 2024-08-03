// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/3/CGL_3_B
#include <iostream>
#include "src/Geometry/Polygon.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 int n;
 cin >> n;
 vector<Point<int>> ps(n);
 for (int i= 0; i < n; ++i) cin >> ps[i];
 Polygon g(ps);
 cout << g.is_convex() << '\n';
 return 0;
}