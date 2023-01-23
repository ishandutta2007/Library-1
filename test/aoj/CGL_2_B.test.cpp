#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/2/CGL_2_B"
#include <iostream>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geometry;
 int q;
 cin >> q;
 while (q--) {
  Segment s, t;
  cin >> s.p1 >> s.p2 >> t.p1 >> t.p2;
  cout << (cross_points(s, t).size() > 0) << endl;
 }
 return 0;
}