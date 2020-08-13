/**
 * @title 共通部分の面積
 * @category 幾何
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/_geometry_temp.hpp"
#undef call_from_test
#endif

namespace geometry {

Real intersection_area(Circle c, Circle d) {
  if (c.r < d.r) swap(c, d);
  auto A = [&](Real r, Real h) {
    return r * r * acos(h / r) - h * sqrt(r * r - h * h);
  };
  Real l = dist(c.o, d.o);
  Real a = (l * l + c.r * c.r - d.r * d.r) / (2 * l);
  if (sgn(l - c.r - d.r) >= 0) return 0;  // far away
  if (sgn(l - c.r + d.r) <= 0) return PI * d.r * d.r;
  if (sgn(l - c.r) >= 0)
    return A(c.r, a) + A(d.r, l - a);
  else
    return A(c.r, a) + PI * d.r * d.r - A(d.r, a - l);
}

Real intersection_area(Polygon g, Circle c) {
  auto tri = [&](Point p, Point q) {
    Point d = q - p;
    Real a = dot(d, p) / dot(d, d), b = (dot(p, p) - c.r * c.r) / dot(d, d);
    Real det = a * a - b;
    if (det <= 0) return arg(p, q) * c.r * c.r / 2;
    Real s = max((Real)0., -a - sqrt(det)), t = min((Real)1., -a + sqrt(det));
    if (t < 0 || 1 <= s) return c.r * c.r * arg(p, q) / 2;
    Point u = p + s * d, v = p + t * d;
    return arg(p, u) * c.r * c.r / 2 + cross(u, v) / 2
           + arg(v, q) * c.r * c.r / 2;
  };
  Real sum = 0.0;
  for (int i = 0; i < g.size(); ++i) sum += tri(g[i] - c.o, g[g.next(i)] - c.o);
  return sum;
}
Real intersection_area(Circle c, Polygon g) { return intersection_area(g, c); }
}  // namespace geometry
