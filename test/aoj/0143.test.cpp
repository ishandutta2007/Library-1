#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0143"
#include <iostream>
#include <vector>
#include "src/Geometry/Polygon.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace geo;
 int n;
 cin >> n;
 while (n--) {
  Point<int> p1, p2, p3, k, s;
  cin >> p1 >> p2 >> p3 >> k >> s;
  Polygon<int> g({p1, p2, p3});
  cout << (g.where(k) * g.where(s) < 0 ? "OK" : "NG") << '\n';
 }
 return 0;
}