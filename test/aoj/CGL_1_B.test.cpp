#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/1/CGL_1_B"
#define ERROR "0.00000001"
#include <iostream>
#include <iomanip>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geometry;
 cout << fixed << setprecision(12);
 Line l;
 cin >> l.p1 >> l.p2;
 int q;
 cin >> q;
 while (q--) {
  Point p;
  cin >> p;
  cout << l.reflect(p) << endl;
 }
 return 0;
}