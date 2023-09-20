#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0012"
#include <iostream>
#include "src/Geometry/Polygon.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace geo;
 Point<long double> p1, p2, p3, p;
 while (cin >> p1 >> p2 >> p3 >> p) {
  Polygon<long double> g({p1, p2, p3});
  cout << (g.where(p) == 1 ? "YES" : "NO") << '\n';
 }
 return 0;
}