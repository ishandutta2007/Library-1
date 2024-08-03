// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_D
// competitive-verifier: ERROR 0.00000001
#include <iostream>
#include <iomanip>
#include "src/Geometry/Circle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 cout << fixed << setprecision(12);
 using R= long double;
 Circle<R> c;
 cin >> c.o >> c.r;
 int q;
 cin >> q;
 while (q--) {
  Point<R> lp, lq;
  cin >> lp >> lq;
  auto ps= cross_points(c, line_through(lp, lq));
  Point p1= ps[0];
  Point p2= ps.size() > 1 ? ps[1] : ps[0];
  if (p2 < p1) swap(p1, p2);
  cout << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << '\n';
 }
 return 0;
}