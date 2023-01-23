#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/3/CGL_3_A"
#define ERROR "0.00000001"
#include <iostream>
#include <iomanip>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geometry;
 int n;
 cin >> n;
 Polygon g(n);
 for (int i= 0; i < n; i++) cin >> g[i];
 cout << fixed << setprecision(12) << g.area() << endl;
 return 0;
}