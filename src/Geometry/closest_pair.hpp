#pragma once
#include <vector>
#include <algorithm>
#include "src/Geometry/Point.hpp"
namespace geo {
template <class K> pair<Point<K>, Point<K>> closest_pair(vector<Point<K>> ps) {
 int n= ps.size();
 assert(n >= 2);
 sort(ps.begin(), ps.end(), [](const Point<K> &p, const Point<K> &q) { return p.y < q.y; });
 vector<Point<K>> memo(n);
 pair<Point<K>, Point<K>> ret= {ps[0], ps[1]};
 K best= dist2(ps[0], ps[1]), tmp;
 auto rec= [&](auto &rec, int l, int r) -> void {
  if (r - l == 1) return;
  int m= (l + r) / 2;
  K y= ps[m].y, d;
  rec(rec, l, m), rec(rec, m, r), inplace_merge(ps.begin() + l, ps.begin() + m, ps.begin() + r);
  for (int i= l, cnt= 0; i < r; ++i) {
   if (d= ps[i].y - y; d * d >= best) continue;
   for (int j= cnt; j--;) {
    if (d= ps[i].x - memo[j].x, tmp= d * d; tmp >= best) break;
    if (d= ps[i].y - memo[j].y, tmp+= d * d; best > tmp) best= tmp, ret= {ps[i], memo[j]};
   }
   memo[cnt++]= ps[i];
  }
 };
 return rec(rec, 0, n), ret;
}
}