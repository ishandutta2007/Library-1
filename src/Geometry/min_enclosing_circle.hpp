/**
 * @title 最小包含円
 * @category 幾何
 * 期待値 O(N)
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "src/Geometry/_geometry_temp.hpp"
#undef call_from_test
#endif

namespace geometry {

Circle min_enclosing_circle(vector<Point> ps) {
  assert(ps.size());
  if (ps.size() == 1) return {ps[0], 0.};
  random_device seed_gen;
  mt19937 mt(seed_gen());
  shuffle(ps.begin(), ps.end(), mt);
  Circle c = {(ps[0] + ps[1]) / 2, dist(ps[0], ps[1]) / 2};
  for (int i = 2; i < ps.size(); i++)
    if (c.contains(ps[i]) == OUT) {
      c = {(ps[0] + ps[i]) / 2, dist(ps[0], ps[i]) / 2};
      for (int j = 1; j < i; j++)
        if (c.contains(ps[j]) == OUT) {
          c = {(ps[i] + ps[j]) / 2, dist(ps[i], ps[j]) / 2};
          for (int k = 0; k < j; k++)
            if (c.contains(ps[k]) == OUT)
              c = circumscribed_circle(ps[i], ps[j], ps[k]);
        }
    }
  return c;
}

}  // namespace geometry
