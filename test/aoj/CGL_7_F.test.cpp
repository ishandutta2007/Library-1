// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_F
// competitive-verifier: ERROR 0.00000001
#include <iostream>
#include <iomanip>
#include "src/Geometry/Circle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 using R= long double;
 Point<R> p;
 cin >> p;
 Circle<R> c;
 cin >> c.o >> c.r;
 auto ls= c.tangent(p);
 Point p1= cross_points(c, ls[0])[0], p2= cross_points(c, ls[1])[0];
 if (p2 < p1) swap(p1, p2);
 cout << fixed << setprecision(12) << p1.x << " " << p1.y << '\n' << p2.x << " " << p2.y << '\n';
 return 0;
}