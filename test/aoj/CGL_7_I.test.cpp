// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_I
// competitive-verifier: ERROR 0.000001
#include <iostream>
#include <iomanip>
#include "src/Geometry/intersection_area.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace geo;
 Circle<long double> c, d;
 cin >> c.o >> c.r >> d.o >> d.r;
 cout << fixed << setprecision(12) << intersection_area(c, d) << '\n';
 return 0;
}