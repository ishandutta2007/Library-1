#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/2/CGL_2_D"
#define ERROR "0.00000001"
#include <iostream>
#include <iomanip>
#include "src/Geometry/Segment.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geo;
 cout << fixed << setprecision(12);
 int q;
 cin >> q;
 while (q--) {
  Segment<long double> s, t;
  cin >> s.p >> s.q >> t.p >> t.q;
  cout << dist(s, t) << '\n';
 }
 return 0;
}