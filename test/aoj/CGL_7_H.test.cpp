#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/7/CGL_7_H"
#define ERROR "0.00000001"
#include <iostream>
#include <iomanip>
#include "src/Geometry/!geometry_temp.hpp"
#include "src/Geometry/circle_functions.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geometry;
 Circle c;
 int n;
 cin >> n >> c.r;
 c.o= {0, 0};
 Polygon g(n);
 for (int i= 0; i < n; i++) cin >> g[i];
 cout << fixed << setprecision(12) << intersection_area(c, g) << endl;
 return 0;
}