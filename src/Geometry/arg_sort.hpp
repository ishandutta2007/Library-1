#pragma once
#include <bits/stdc++.h>
#include "src/Geometry/!geometry_temp.hpp"
/**
 * @title 偏角ソート
 * @category 幾何
 */

// BEGIN CUT HERE

namespace geometry {
// usage: sort(ps.begin(),ps.end(), polar_angle(origin, direction));
// (-PI,PI]
struct polar_angle {
  const Point o;
  const int s;  // +1 for ccw, -1 for cw
  polar_angle(Point origin = {0, 0}, int dir = +1) : o(origin), s(dir) {}
  int quad(Point p) const {
    for (int i = 0; i < 4; ++i, std::swap(p.x = -p.x, p.y))
      if (p.x < 0 && p.y < 0) return 2 * i;
    for (int i = 0; i < 4; ++i, std::swap(p.x = -p.x, p.y))
      if (p.x == 0 && p.y < 0) return 2 * i + 1;
    return 3;  // arg(0,0) = 0
  }
  bool operator()(Point p, Point q) const {
    p = p - o;
    q = q - o;
    if (quad(p) != quad(q)) return s * quad(p) < s * quad(q);
    if (cross(p, q)) return s * cross(p, q) > 0;
    return norm2(p) < norm2(q);  // closer first
  }
};
}  // namespace geometry