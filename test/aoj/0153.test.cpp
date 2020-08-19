#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=0153"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/!geometry_temp.hpp"
#include "src/Geometry/circle_functions.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using namespace geometry;
  for (int a, b; cin >> a >> b && a;) {
    Convex tri(3);
    tri[0] = {Real(a), Real(b)};
    cin >> tri[1] >> tri[2];
    if (ccw(tri[0], tri[1], tri[2]) == CLOCKWISE) swap(tri[1], tri[2]);
    Circle c;
    cin >> c.o >> c.r;
    Real S = intersection_area(c, tri);
    if (sgn(S - c.area()) == 0)
      cout << "a" << endl;
    else if (sgn(S - tri.area()) == 0)
      cout << "b" << endl;
    else if (sgn(S) || sgn(dist(c.o, tri) - c.r) <= 0)
      cout << "c" << endl;
    else
      cout << "d" << endl;
  }
  return 0;
}