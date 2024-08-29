// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_B
// competitive-verifier: ERROR 0.00000001
// competitive-verifier: TLE 0.5
#include <iostream>
#include <iomanip>
#include "src/Geometry/Circle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 Point<long double> A, B, C;
 cin >> A >> B >> C;
 Circle c= inscribed_circle(A, B, C);
 cout << fixed << setprecision(12) << c.o.x << " " << c.o.y << " " << c.r << '\n';
 return 0;
}