// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_E
// competitive-verifier: ERROR 0.00000001
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <iomanip>
#include "src/Geometry/Circle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 Circle<long double> c, d;
 cin >> c.o >> c.r;
 cin >> d.o >> d.r;
 auto ps= cross_points(c, d);
 Point p1= ps[0];
 Point p2= ps.size() > 1 ? ps[1] : ps[0];
 if (p2 < p1) swap(p1, p2);
 cout << fixed << setprecision(12) << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << '\n';
 return 0;
}