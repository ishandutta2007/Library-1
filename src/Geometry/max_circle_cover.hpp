/**
 * @title 半径固定の円の最大被覆点数
 * @category 幾何
 * ４分木
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/!geometry_temp.hpp"
#undef call_from_test
#endif

namespace geometry {

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