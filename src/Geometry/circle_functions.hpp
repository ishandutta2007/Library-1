/**
 * @title 円のあれこれ
 * @category 幾何
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/!geometry_temp.hpp"
#undef call_from_test
#endif

namespace geometry {

Circle inscribed_circle(Point A, Point B, Point C) {
  Real a = dist(B, C), b = dist(C, A), c = dist(A, B);
  Real s = (a + b + c) / 2;
  Point o = (a * A + b * B + c * C) / (a + b + c);
  Real r = sqrt((s - a) * (s - b) * (s - c) / s);
  return {o, r};
}

Circle circumscribed_circle(Point A, Point B, Point C) {
  Point u = orth(B - A), v = C - A;
  Point o = (A + B + u * dot(C - B, v) / dot(u, v)) / 2;
  return {o, dist(A, o)};
}

vector<Line> common_tangent(Circle c, Circle d) {
  Real len = dist(c.o, d.o);
  if (sgn(len) == 0) return {};  // same origin
  Point u = (d.o - c.o) / len, v = orth(u);
  vector<Line> ls;
  for (int s = +1; s >= -1; s -= 2) {
    Real h = (c.r + s * d.r) / len;
    if (sgn(1 - h * h) == 0) {  // touch inner/outer
      ls.emplace_back(Line{c.o + h * c.r * u, c.o + h * c.r * (u + v)});
    } else if (sgn(1 - h * h) > 0) {  // properly intersect
      Point uu = h * u, vv = sqrt(1 - h * h) * v;
      ls.emplace_back(Line{c.o + c.r * (uu + vv), d.o - d.r * (uu + vv) * s});
      ls.emplace_back(Line{c.o + c.r * (uu - vv), d.o - d.r * (uu - vv) * s});
    }
  }
  return ls;
}

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
  for (int i = 0; i < (int)g.size(); ++i)
    sum += tri(g[i] - c.o, g[g.next(i)] - c.o);
  return sum;
}
Real intersection_area(Circle c, Polygon g) { return intersection_area(g, c); }

Circle min_enclosing_circle(vector<Point> ps) {
  assert(ps.size());
  if (ps.size() == 1) return {ps[0], 0.};
  random_device seed_gen;
  mt19937 mt(seed_gen());
  shuffle(ps.begin(), ps.end(), mt);
  Circle c = {(ps[0] + ps[1]) / 2, dist(ps[0], ps[1]) / 2};
  for (int i = 2; i < ps.size(); i++)
    if (c.where(ps[i]) == OUT) {
      c = {(ps[0] + ps[i]) / 2, dist(ps[0], ps[i]) / 2};
      for (int j = 1; j < i; j++)
        if (c.where(ps[j]) == OUT) {
          c = {(ps[i] + ps[j]) / 2, dist(ps[i], ps[j]) / 2};
          for (int k = 0; k < j; k++)
            if (c.where(ps[k]) == OUT)
              c = circumscribed_circle(ps[i], ps[j], ps[k]);
        }
    }
  return c;
}

pair<int, Point> max_circle_cover(vector<Point> ps, Real r) {
  const Real dx[4] = {1, -1, -1, 1}, dy[4] = {1, 1, -1, -1};
  Point best_p;
  int best = 0;
  function<void(Point, Real, vector<Point>)> rec
      = [&](Point p, Real w, vector<Point> ps) {
          w /= 2;
          Point qs[4];
          vector<Point> pss[4];
          for (int i = 0; i < 4; ++i) {
            qs[i] = p + w * Point({dx[i], dy[i]});
            int lo = 0;
            for (Point q : ps) {
              Real d = dist(qs[i], q);
              if (sgn(d - r) <= 0) ++lo;
              if (sgn(d - w * sqrt(2) - r) <= 0) pss[i].push_back(q);
            }
            if (lo > best) {
              best = lo;
              best_p = qs[i];
            }
          }
          for (int i = 0; i < 4; ++i) {
            for (int j = i + 1; j < 4; ++j)
              if (pss[i].size() < pss[j].size())
                swap(pss[i], pss[j]), swap(qs[i], qs[j]);
            if (pss[i].size() <= best) break;
            rec(qs[i], w, pss[i]);
          }
        };
  Real w = 0;
  for (Point p : ps) w = max({w, abs(p.x), abs(p.y)});
  rec({0, 0}, w, ps);
  return {best, best_p};
}

}  // namespace geometry
