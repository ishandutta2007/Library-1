#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_A"
#include <iostream>
#include "src/Geometry/Circle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 Circle<long double> c, d;
 cin >> c.o >> c.r;
 cin >> d.o >> d.r;
 cout << common_tangent(c, d).size() << '\n';
 return 0;
}