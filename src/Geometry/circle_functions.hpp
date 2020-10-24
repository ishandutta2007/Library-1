#pragma once
#include <bits/stdc++.h>
#include "src/Geometry/!geometry_temp.hpp"
/**
 * @title 円のあれこれ
 * @category 幾何
 */

// BEGIN CUT HERE

namespace geometry {

Circle inscribed_circle(Point A, Point B, Point C) {
  Real a = dist(B, C), b = dist(C, A), c = dist(A, B);
  Real s = (a + b + c) / 2;
  Point o = (a * A + b * B + c * C) / (a + b + c);
  Real r = std::sqrt((s - a) * (s - b) * (s - c) / s);
  return {o, r};
}

Circle circumscribed_circle(Point A, Point B, Point C) {
  Point u = orth(B - A), v = C - A;
  Point o = (A + B + u * dot(C - B, v) / dot(u, v)) / 2;
  return {o, dist(A, o)};
}

std::vector<Line> common_tangent(Circle c, Circle d) {
  Real len = dist(c.o, d.o);
  if (sgn(len) == 0) return {};  // same origin
  Point u = (d.o - c.o) / len, v = orth(u);
  std::vector<Line> ls;
  for (int s = +1; s >= -1; s -= 2) {
    Real h = (c.r + s * d.r) / len;
    if (sgn(1 - h * h) == 0) {  // touch inner/outer
      ls.emplace_back(Line{c.o + h * c.r * u, c.o + h * c.r * (u + v)});
    } else if (sgn(1 - h * h) > 0) {  // properly intersect
      Point uu = h * u, vv = std::sqrt(1 - h * h) * v;
      ls.emplace_back(Line{c.o + c.r * (uu + vv), d.o - d.r * (uu + vv) * s});
      ls.emplace_back(Line{c.o + c.r * (uu - vv), d.o - d.r * (uu - vv) * s});
    }
  }
  return ls;
}

Real intersection_area(Circle c, Circle d) {
  if (c.r < d.r) std::swap(c, d);
  auto A = [&](Real r, Real h) {
    return r * r * std::acos(h / r) - h * std::sqrt(r * r - h * h);
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
    Real s = std::max((Real)0., -a - std::sqrt(det)),
         t = std::min((Real)1., -a + std::sqrt(det));
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

Circle min_enclosing_circle(std::vector<Point> ps) {
  assert(ps.size());
  if (ps.size() == 1) return {ps[0], 0.};
  std::random_device seed_gen;
  std::mt19937 mt(seed_gen());
  std::shuffle(ps.begin(), ps.end(), mt);
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

std::pair<int, Point> max_circle_cover(std::vector<Point> ps, Real r) {
  const Real dx[4] = {1, -1, -1, 1}, dy[4] = {1, 1, -1, -1};
  Point best_p;
  int best = 0;
  std::function<void(Point, Real, std::vector<Point>)> rec
      = [&](Point p, Real w, std::vector<Point> ps) {
          w /= 2;
          Point qs[4];
          std::vector<Point> pss[4];
          for (int i = 0; i < 4; ++i) {
            qs[i] = p + w * Point({dx[i], dy[i]});
            int lo = 0;
            for (Point q : ps) {
              Real d = dist(qs[i], q);
              if (sgn(d - r) <= 0) ++lo;
              if (sgn(d - w * std::sqrt(2) - r) <= 0) pss[i].push_back(q);
            }
            if (lo > best) {
              best = lo;
              best_p = qs[i];
            }
          }
          for (int i = 0; i < 4; ++i) {
            for (int j = i + 1; j < 4; ++j)
              if (pss[i].size() < pss[j].size())
                std::swap(pss[i], pss[j]), std::swap(qs[i], qs[j]);
            if ((int)pss[i].size() <= best) break;
            rec(qs[i], w, pss[i]);
          }
        };
  Real w = 0;
  for (Point p : ps) w = std::max({w, std::abs(p.x), std::abs(p.y)});
  rec({0, 0}, w, ps);
  return {best, best_p};
}

}  // namespace geometry
