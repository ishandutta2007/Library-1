// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/1/CGL_1_C
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/Geometry/Point.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using P= Point<int>;
 P p0, p1;
 cin >> p0 >> p1;
 int q;
 cin >> q;
 while (q--) {
  P p2;
  cin >> p2;
  cout << ccw(p0, p1, p2) << '\n';
 }
 return 0;
}