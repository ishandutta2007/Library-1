#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0153"
#include <iostream>
#include "src/Geometry/Polygon.hpp"
#include "src/Geometry/Circle.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using namespace geo;
 for (int x, y; cin >> x >> y && x;) {
  Point<int> p1(x, y), p2, p3;
  cin >> p2 >> p3;
  Polygon<int> g({p1, p2, p3});
  Circle<int> c;
  cin >> c.o >> c.r;
  if (min({c.where(p1), c.where(p2), c.where(p3)}) != -1) {
   cout << "b" << '\n';
   continue;
  }
  int d= 1e9;
  for (const auto &e: g.edges()) d= min(d, dist2(e, c.o));
  if (int s= d - c.r * c.r; g.where(c.o) != -1 && s >= 0) cout << "a" << '\n';
  else if (s <= 0) cout << "c" << '\n';
  else cout << "d" << '\n';
 }
 return 0;
}