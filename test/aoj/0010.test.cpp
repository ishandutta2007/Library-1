#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0010"
#include <iostream>
#include "src/Geometry/Circle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace geo;
 cout << fixed << setprecision(3);
 int n;
 cin >> n;
 while (n--) {
  Point<long double> p1, p2, p3;
  cin >> p1 >> p2 >> p3;
  auto c= circumscribed_circle(p1, p2, p3);
  cout << c.o.x << " " << c.o.y << " " << c.r << '\n';
 }
 return 0;
}