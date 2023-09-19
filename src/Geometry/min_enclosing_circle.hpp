#pragma once
#include "src/Misc/rng.hpp"
#include "src/Geometry/Circle.hpp"
namespace geo {
template <class R> Circle<R> min_enclosing_circle(vector<Point<R>> ps) {
 assert(ps.size());
 if (ps.size() == 1) return {ps[0], 0.};
 const int n= ps.size();
 for (int i= n; --i;) swap(ps[i], ps[rng(i + 1)]);
 Circle c= {(ps[0] + ps[1]) / 2, dist(ps[0], ps[1]) / 2};
 for (int i= 2; i < n; ++i)
  if (c.where(ps[i]) == -1) {
   c= {(ps[0] + ps[i]) / 2, dist(ps[0], ps[i]) / 2};
   for (int j= 1; j < i; ++j)
    if (c.where(ps[j]) == -1) {
     c= {(ps[i] + ps[j]) / 2, dist(ps[i], ps[j]) / 2};
     for (int k= 0; k < j; ++k)
      if (c.where(ps[k]) == -1) c= circumscribed_circle(ps[i], ps[j], ps[k]);
    }
  }
 return c;
}
}