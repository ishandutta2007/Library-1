/**
 * @title 最近点対
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
pair<Point, Point> closest_pair(vector<Point> ps) {
  sort(ps.begin(), ps.end(), [](Point p, Point q) { return p.y < q.y; });
  Point u = ps[0], v = ps[1];
  Real best = norm2(u - v);
  auto update = [&](Point p, Point q) {
    Real dis = norm2(p - q);
    if (best > dis) best = dis, u = p, v = q;
  };
  function<void(int, int)> rec = [&](int l, int r) {
    if (r - l <= 1) {
      for (int i = l; i < r; ++i)
        for (int j = i + 1; j < r; ++j) update(ps[i], ps[j]);
      stable_sort(&ps[l], &ps[r]);
    } else {
      int m = (l + r) / 2;
      Real y = ps[m].y;
      rec(l, m);
      rec(m, r);
      inplace_merge(&ps[l], &ps[m], &ps[r]);
      vector<Point> qs;
      for (int i = l; i < r; ++i) {
        if ((ps[i].y - y) * (ps[i].y - y) >= best) continue;
        for (int j = (int)qs.size() - 1; j >= 0; --j) {
          if ((qs[j].x - ps[i].x) * (qs[j].x - ps[i].x) >= best) break;
          update(qs[j], ps[i]);
        }
        qs.push_back(ps[i]);
      }
    }
  };
  rec(0, ps.size());
  return {u, v};
}

pair<Point, Point> farthest_pair(vector<Point> ps) {
  return convex_hull(ps).farthest();
}
}  // namespace geometry