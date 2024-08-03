// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/1/CGL_1_A
// competitive-verifier: ERROR 0.00000001
#include <iostream>
#include <iomanip>
#include "src/Geometry/Line.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 cout << fixed << setprecision(12);
 using P= Point<long double>;
 P p1, p2;
 cin >> p1 >> p2;
 Line l= line_through(p1, p2);
 int q;
 cin >> q;
 while (q--) {
  P p;
  cin >> p;
  auto ans= l.project(p);
  cout << ans.x << " " << ans.y << '\n';
 }
 return 0;
}